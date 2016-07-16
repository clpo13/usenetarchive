#include <algorithm>
#include <ctype.h>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#ifdef _WIN32
#  include <malloc.h>
#else
#  include <alloca.h>
#endif

#include <unicode/locid.h>
#include <unicode/brkiter.h>
#include <unicode/unistr.h>

#include "../common/MetaView.hpp"
#include "../common/MessageView.hpp"
#include "../common/String.hpp"

const char* AllowedHeaders[] = {
    "from",
    "subject",
    nullptr
};

bool IsHeaderAllowed( const char* hdr, const char* end )
{
    int size = end - hdr;
    char* tmp = (char*)alloca( size+1 );
    for( int i=0; i<size; i++ )
    {
        tmp[i] = tolower( hdr[i] );
    }
    tmp[size] = '\0';

    auto test = AllowedHeaders;
    while( *test )
    {
        if( strncmp( tmp, *test, size+1 ) == 0 )
        {
            return true;
        }
        test++;
    }
    return false;
}

UErrorCode wordItErr = U_ZERO_ERROR;
auto wordIt = icu::BreakIterator::createWordInstance( icu::Locale::getEnglish(), wordItErr );

void SplitLine( const char* ptr, const char* end, std::vector<std::string>& out )
{
    out.clear();
    auto us = icu::UnicodeString::fromUTF8( StringPiece( ptr, end-ptr ) );
    auto lower = us.toLower( icu::Locale::getEnglish() );

    wordIt->setText( lower );
    int32_t p0 = 0;
    int32_t p1 = wordIt->first();
    while( p1 != icu::BreakIterator::DONE )
    {
        auto part = lower.tempSubStringBetween( p0, p1 );
        std::string str;
        part.toUTF8String( str );
        if( str.size() > 2 && str.size() < 14 )
        {
            out.emplace_back( std::move( str ) );
        }
        p0 = p1;
        p1 = wordIt->next();
    }
}

enum Type
{
    T_Content,
    T_Signature,
    T_Quote1,
    T_Quote2,
    T_Quote3,
    T_Header
};

using HitData = std::map<std::string, std::map<uint32_t, std::vector<uint16_t>>>;

enum { MaxChildren = 0x1F };

void Add( HitData& data, const std::vector<std::string>& words, uint32_t idx, int type, int basePos, int childCount )
{
    for( auto& w : words )
    {
        uint16_t hit = std::min( 0xFF, basePos++ ) | ( childCount << 8 ) | ( type << 13 );
        auto& hits = data[w];
        hits[idx].emplace_back( hit );
    }
}

void CountChildren( MetaView<uint32_t, uint32_t>& conn, uint32_t idx, int& cnt )
{
    if( ++cnt == MaxChildren ) return;
    auto data = conn[idx];
    data += 2;
    auto num = *data++;
    for( int i=0; i<num; i++ )
    {
        CountChildren( conn, *data++, cnt );
        if( cnt == MaxChildren ) return;
    }
}

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        fprintf( stderr, "USAGE: %s raw\n", argv[0] );
        exit( 1 );
    }

    std::string base = argv[1];
    base.append( "/" );

    MessageView mview( base + "meta", base + "data" );
    MetaView<uint32_t, uint32_t> conn( base + "connmeta", base + "conndata" );
    const auto size = mview.Size();
    std::vector<std::string> wordbuf;
    HitData data;

    for( uint32_t i=0; i<size; i++ )
    {
        if( ( i & 0x3FF ) == 0 )
        {
            printf( "%i/%i\r", i, size );
            fflush( stdout );
        }

        bool headers = true;
        bool signature = false;
        int basePos[5] = {};

        int children = -1;
        CountChildren( conn, i, children );
        auto post = mview[i];
        for(;;)
        {
            auto end = post;
            if( headers )
            {
                if( *end == '\n' )
                {
                    headers = false;
                    continue;
                }
                while( *end != ':' ) end++;
                end += 2;
                if( IsHeaderAllowed( post, end-2 ) )
                {
                    const char* line = end;
                    while( *end != '\n' ) end++;
                    SplitLine( line, end, wordbuf );
                    Add( data, wordbuf, i, T_Header, 0, children );
                }
                else
                {
                    while( *end != '\n' ) end++;
                }
                post = end + 1;
            }
            else
            {
                const char* line = end;
                int quotLevel = 0;
                while( *end != '\n' && *end != '\0' ) end++;
                if( end - line == 4 && strncmp( line, "-- ", 3 ) == 0 )
                {
                    signature = true;
                }
                else
                {
                    while( *line == ' ' || *line == '>' || *line == ':' || *line == '|' || *line == '\t' )
                    {
                        if( *line == '>' || *line == ':' || *line == '|' )
                        {
                            quotLevel++;
                        }
                        line++;
                    }
                }
                if( line != end )
                {
                    SplitLine( line, end, wordbuf );
                    Type t;
                    if( signature )
                    {
                        t = T_Signature;
                    }
                    else
                    {
                        switch( quotLevel )
                        {
                        case 0:
                            t = T_Content;
                            break;
                        case 1:
                            t = T_Quote1;
                            break;
                        case 2:
                            t = T_Quote2;
                            break;
                        default:
                            t = T_Quote3;
                            break;
                        }
                    }
                    Add( data, wordbuf, i, t, basePos[t], children );
                    basePos[t] += wordbuf.size();
                }
                if( *end == '\0' ) break;
                post = end + 1;
            }
        }
    }

    printf( "\n" );

    return 0;
}
