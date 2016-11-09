#include <assert.h>
#include <stdlib.h>
#include "../libuat/Archive.hpp"

#include "ThreadView.hpp"

ThreadView::ThreadView( const Archive& archive )
    : View( 0, 1, 0, -2 )
    , m_archive( archive )
    , m_data( archive.NumberOfMessages() )
    , m_top( 0 )
    , m_cursor( 0 )
{
    unsigned int idx = 0;
    const auto toplevel = archive.GetTopLevel();
    for( int i=0; i<toplevel.size; i++ )
    {
        Fill( idx, toplevel.ptr[i] );
        idx += archive.GetTotalChildrenCount( toplevel.ptr[i] );
    }

    Draw();
}

ThreadView::~ThreadView()
{
}

void ThreadView::Resize()
{
    ResizeView( 0, 1, 0, -2 );
    wclear( m_win );
    Draw();
}

void ThreadView::Draw()
{
    int w, h;
    getmaxyx( m_win, h, w );

    werase( m_win );

    auto idx = m_top;
    for( int i=0; i<h; i++ )
    {
        assert( m_data[idx].valid == 1 );
        DrawLine( idx );
        if( m_data[idx].expanded )
        {
            idx++;
        }
        else
        {
            idx += m_archive.GetTotalChildrenCount( m_data[idx].msgid );
        }
    }

    wnoutrefresh( m_win );
}

void ThreadView::Fill( int index, int msgid )
{
    assert( m_data[index].valid == 0 );
    m_data[index].msgid = msgid;
    m_data[index].valid = 1;
}

void ThreadView::DrawLine( int idx )
{
    const auto midx = m_data[idx].msgid;
    wchar_t buf[1024];
    if( m_cursor == idx )
    {
        wattron( m_win, COLOR_PAIR(2) | A_BOLD );
        wprintw( m_win, "->" );
        wattroff( m_win, COLOR_PAIR(2) | A_BOLD );
    }
    else
    {
        wprintw( m_win, "  " );
    }

    const auto children = m_archive.GetTotalChildrenCount( midx );
    if( children > 9999 )
    {
        wprintw( m_win, "++++ [" );
    }
    else
    {
        wprintw( m_win, "%4i [", children );
    }

    mbstowcs( buf, m_archive.GetRealName( midx ), 1024 );
    wattron( m_win, COLOR_PAIR(3) | A_BOLD );
    buf[18] = L'\0';
    wprintw( m_win, "%ls", buf );
    auto len = wcslen( buf );
    for( int i=len; i<18; i++ )
    {
        waddch( m_win, ' ' );
    }
    wattroff( m_win, COLOR_PAIR(3) | A_BOLD );
    wprintw( m_win, "] " );

    if( children == 1 )
    {
        wprintw( m_win, "  " );
    }
    else
    {
        wattron( m_win, COLOR_PAIR(4) );
        waddch( m_win, m_data[idx].expanded ? '-' : '+' );
        waddch( m_win, ' ' );
        wattroff( m_win, COLOR_PAIR(4) );
    }

    mbstowcs( buf, m_archive.GetSubject( midx ), 1024 );
    wprintw( m_win, "%ls\n", buf );
}
