#ifndef __THREADVIEW_HPP__
#define __THREADVIEW_HPP__

#include <stdint.h>
#include <vector>

#include "GalaxyState.hpp"
#include "ThreadTree.hpp"
#include "View.hpp"

class Archive;
class Galaxy;
class MessageView;
class PersistentStorage;

class ThreadView : public View
{
public:
    ThreadView( const Archive& archive, PersistentStorage& storage, const Galaxy* galaxy, const MessageView& mview );

    void Reset( const Archive& archive );

    void Resize();
    void Draw();

    void MoveCursor( int offset );
    void GoNextUnread();
    bool CanExpand( int cursor );
    int Expand( int cursor, bool recursive );
    void Collapse( int cursor );
    bool IsExpanded( int cursor ) const { return m_tree.IsExpanded( cursor ); }
    void FocusOn( int cursor );
    void MarkTreeCondensed( int cursor, int depth );

    int GetCursor() const { return m_cursor; }
    void SetCursor( int cursor ) { m_cursor = cursor; }
    int GetRoot( int cursor ) const;

    void PageForward();
    void PageBackward();
    void MoveCursorTop();
    void MoveCursorBottom();

    GalaxyState CheckGalaxyState( int cursor ) const;

private:
    void ExpandFill( int cursor );
    bool DrawLine( int line, int idx, const char*& prev );

    int GetNext( int idx );
    int GetPrev( int idx ) const;

    bool CheckVisited( int idx );
    ScoreState GetScoreState( int idx );
    GalaxyState GetGalaxyState( int idx );

    const Archive* m_archive;
    PersistentStorage& m_storage;
    const Galaxy* m_galaxy;

    const MessageView& m_mview;
    ThreadTree m_tree;
    int m_top, m_bottom;
    int m_cursor;
    int m_fillPos, m_topLevelPos;
};

#endif
