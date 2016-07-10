#include <stdlib.h>
#include <stdio.h>

#include "../contrib/linenoise-ng/linenoise.h"

#include "../common/Filesystem.hpp"
#include "../libuat/Archive.hpp"

void PrintHelp()
{
    printf( "info        - archive info\n" );
    printf( "toplevel    - list toplevel messages\n" );
    printf( "view idx    - view message of given idx\n" );
}

void Info( const Archive& archive )
{
    printf( "Number of messages: %i\n", archive.NumberOfMessages() );
    printf( "Number of toplevel messages: %i\n", archive.NumberOfTopLevel() );
}

int main( int argc, char** argv )
{
    if( argc < 2 )
    {
        fprintf( stderr, "USAGE: %s archive\n", argv[0] );
        exit( 1 );
    }
    if( !Exists( argv[1] ) )
    {
        fprintf( stderr, "Archive doesn't exist.\n" );
        exit( 1 );
    }

    Archive archive( argv[1] );

    printf( "Usenet archive %s opened.\n", argv[1] );
    Info( archive );

    while( char* cmd = linenoise( "\x1b[1;32mcmd>\x1b[0m " ) )
    {
        if( strncmp( cmd, "view ", 5 ) == 0 )
        {
            int idx = atoi( cmd+5 );
            auto msg = archive.GetMessage( idx );
            if( msg )
            {
                printf( "%s\n", msg );
            }
            else
            {
                printf( "Invalid message index (max %i).\n", archive.NumberOfMessages() );
            }
        }
        else if( strcmp( cmd, "toplevel" ) == 0 )
        {
            auto view = archive.GetTopLevel();
            for( uint64_t i=0; i<view.size; i++ )
            {
                printf( "%i\n", view.ptr[i] );
            }
        }
        else if( strcmp( cmd, "info" ) == 0 )
        {
            Info( archive );
        }
        else
        {
            PrintHelp();
        }
    }

    return 0;
}
