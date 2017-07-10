#ifndef __THREADTREE_HPP__
#define __THREADTREE_HPP__

#include <vector>

#include "BitSet.hpp"
#include "GalaxyState.hpp"
#include "ThreadData.hpp"

class ThreadTree
{
public:
    ThreadTree( size_t size );
    void Reset( size_t size );

    bool IsValid( int idx ) const { return m_data[idx].valid; }
    bool IsExpanded( int idx ) const { return m_data[idx].expanded; }
    bool WasVisited( int idx ) const { return m_data[idx].visited; }
    bool WasAllVisited( int idx ) const { return m_data[idx].visall; }
    ScoreState GetScoreState( int idx ) const { return (ScoreState)m_tree[idx].GetScoreData(); }
    GalaxyState GetGalaxyState( int idx ) const { return (GalaxyState)m_data[idx].galaxy; }
    int GetCondensedValue( int idx ) const { return m_data[idx].condensed; }
    int GetTreeLine( int idx, int line ) const { return m_tree[idx].Get( line ); }
    int GetTreeLineSize( int idx ) const { return m_tree[idx].Size(); }

    void SetValid( int idx, bool val ) { m_data[idx].valid = val; }
    void SetExpanded( int idx, bool val ) { m_data[idx].expanded = val; }
    void SetVisited( int idx, bool val ) { m_data[idx].visited = val; }
    void SetAllVisited( int idx, bool val ) { m_data[idx].visall = val; }
    void SetScoreState( int idx, ScoreState state ) { m_tree[idx].SetScoreData( (int)state ); }
    void SetGalaxyState( int idx, GalaxyState state ) { m_data[idx].galaxy = (uint8_t)state; }
    void SetCondensedValue( int idx, int val ) { m_data[idx].condensed = val; }
    void SetTreeLine( int idx, bool line ) { m_tree[idx].Set( line ); }

private:
    std::vector<ThreadData> m_data;
    std::vector<BitSet> m_tree;
};

#endif
