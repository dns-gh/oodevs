// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_KnowledgeGroupQuerier.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 15:59 $
// $Revision: 8 $
// $Workfile: DEC_KS_KnowledgeGroupQuerier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_KnowledgeGroupQuerier.h"

#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_KS_ArmyQuerier.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeGroupQuerier::DEC_KS_KnowledgeGroupQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pKnowledgeGroup_       ( &knowledgeGroup )
{
        
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_KnowledgeGroupQuerier::~DEC_KS_KnowledgeGroupQuerier()
{
}

// -----------------------------------------------------------------------------
// Name: sPotentiallyDangerousAgentsInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sPotentiallyDangerousAgentsInserter
{
public:
    sPotentiallyDangerousAgentsInserter( const MIL_KnowledgeGroup& knowledgeGroup, T_KnowledgeAgentDiaIDVector& container )
        : pKnowledgeGroup_( &knowledgeGroup )
        , pContainer_     ( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if(    pKnowledgeGroup_->GetArmy().IsAnEnemy( knowledge ) != eTristate_False 
            && !knowledge.IsMilitia() 
            && !knowledge.IsRefugee() )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    const MIL_KnowledgeGroup*   pKnowledgeGroup_;
    T_KnowledgeAgentDiaIDVector* pContainer_;
};
// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetPotentiallyDangerousAgents
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetPotentiallyDangerousAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pKnowledgeGroup_ );     
    sPotentiallyDangerousAgentsInserter functor( *pKnowledgeGroup_, container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );
}


// -----------------------------------------------------------------------------
// Name: sMilitiaAgentsInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sMilitiaAgentsInserter
{
public:
    sMilitiaAgentsInserter( const MIL_KnowledgeGroup& knowledgeGroup, T_KnowledgeAgentDiaIDVector& container )
        : pKnowledgeGroup_( &knowledgeGroup )
        , pContainer_     ( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if ( knowledge.IsMilitia() )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    const MIL_KnowledgeGroup*    pKnowledgeGroup_;
    T_KnowledgeAgentDiaIDVector* pContainer_;
};
// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetMilitiaAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetMilitiaAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pKnowledgeGroup_ );
    sMilitiaAgentsInserter functor( *pKnowledgeGroup_, container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );
}

// -----------------------------------------------------------------------------
// Name: sRefugeeAgentsInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sRefugeeAgentsInserter
{
public:
    sRefugeeAgentsInserter( const MIL_KnowledgeGroup& knowledgeGroup, T_KnowledgeAgentDiaIDVector& container )
        : pKnowledgeGroup_( &knowledgeGroup )
        , pContainer_     ( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if( knowledge.IsRefugee() )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    const MIL_KnowledgeGroup*    pKnowledgeGroup_;
    T_KnowledgeAgentDiaIDVector* pContainer_;
};
// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetRefugeeAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetRefugeeAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pKnowledgeGroup_ );
    sRefugeeAgentsInserter functor( *pKnowledgeGroup_, container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );
}

// -----------------------------------------------------------------------------
// Name: sSurrenderedAgentsInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sSurrenderedAgentsInserter
{
public:
    sSurrenderedAgentsInserter( const MIL_KnowledgeGroup& knowledgeGroup, T_KnowledgeAgentDiaIDVector& container )
        : pKnowledgeGroup_( &knowledgeGroup )
        , pContainer_     ( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        assert( pKnowledgeGroup_ );
        if( knowledge.IsSurrendered() && ( !knowledge.GetArmy() || *knowledge.GetArmy() != pKnowledgeGroup_->GetArmy() ) )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    const MIL_KnowledgeGroup*    pKnowledgeGroup_;
    T_KnowledgeAgentDiaIDVector* pContainer_;
};
// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetSurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetSurrenderedAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pKnowledgeGroup_ );
    sSurrenderedAgentsInserter functor( *pKnowledgeGroup_, container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );
}

// -----------------------------------------------------------------------------
// Name: sEnemyKnowledgesInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sEnemyKnowledgesInserter
{
public:
    sEnemyKnowledgesInserter( const MIL_Army& army, T_KnowledgeAgentVector& container )
        : pArmy_     ( &army )
        , pContainer_( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if( pArmy_->IsAnEnemy( knowledge ) == eTristate_True )
            pContainer_->push_back( &knowledge );
    }

private:
    const MIL_Army*        pArmy_;
    T_KnowledgeAgentVector* pContainer_;
};
// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetEnemies( T_KnowledgeAgentVector& container ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sEnemyKnowledgesInserter functor( pKnowledgeGroup_->GetArmy(), container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );    
}

// -----------------------------------------------------------------------------
// Name: sFriendKnowledgesInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sFriendKnowledgesInserter
{
public:
    sFriendKnowledgesInserter( const MIL_Army& army, T_KnowledgeAgentVector& container )
        : pArmy_     ( &army )
        , pContainer_( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if( pArmy_->IsAFriend( knowledge ) == eTristate_True )
            pContainer_->push_back( &knowledge );
    }

private:
    const MIL_Army*        pArmy_;
    T_KnowledgeAgentVector* pContainer_;
};

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetFriends
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetFriends( T_KnowledgeAgentVector& container ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sFriendKnowledgesInserter functor( pKnowledgeGroup_->GetArmy(), container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );        
}

// -----------------------------------------------------------------------------
// Name: sFriendKnowledgesInZoneInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sFriendKnowledgesInZoneInserter
{
public:
    sFriendKnowledgesInZoneInserter( const MIL_Army& army, const TER_Localisation& zone, T_KnowledgeAgentDiaIDVector& container )
        : pArmy_     ( &army )
        , pZone_     ( &zone )
        , pContainer_( &container )
    {
    }

    void operator() ( DEC_Knowledge_Agent& knowledge )
    {
        if( pArmy_->IsAFriend( knowledge ) == eTristate_True && pZone_->IsInside( knowledge.GetPosition() ) )
            pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    const MIL_Army*              pArmy_;
    const TER_Localisation*      pZone_; 
    T_KnowledgeAgentDiaIDVector* pContainer_;
};

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetFriendsInZone( T_KnowledgeAgentDiaIDVector& container, const TER_Localisation& zone ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sFriendKnowledgesInZoneInserter functor( pKnowledgeGroup_->GetArmy(), zone, container );
    
    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesAgent( functor );     
}

// -----------------------------------------------------------------------------
// Name: sPopulationKnowledgesInserter
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
class sPopulationKnowledgesInserter
{
public:
    sPopulationKnowledgesInserter( T_KnowledgePopulationDiaIDVector& container )
        : pContainer_( &container )
    {
    }

    void operator() ( DEC_Knowledge_Population& knowledge )
    {
        pContainer_->push_back( (void*)knowledge.GetID() );
    }

private:
    T_KnowledgePopulationDiaIDVector* pContainer_;
};

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KS_KnowledgeGroupQuerier::GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sPopulationKnowledgesInserter functor( container );

    assert( pBlackBoard_ );
    pBlackBoard_->ApplyOnKnowledgesPopulation( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgent
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeAgent( const DEC_Knowledge_AgentPerception& perception ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeAgent( perception.GetAgentPerceived() );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeObjectFromID
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeObjectFromID( uint nID ) const
{
    assert( pKnowledgeGroup_ );
    return pKnowledgeGroup_->GetArmy().GetKSQuerier().GetKnowledgeObjectFromID( nID );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgeObject
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_KnowledgeGroupQuerier::GetKnowledgeObject( MIL_RealObject_ABC& object ) const
{
    assert( pKnowledgeGroup_ );
    return pKnowledgeGroup_->GetArmy().GetKSQuerier().GetKnowledgeObject( object );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulation
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulation( const DEC_Knowledge_PopulationCollision& collision ) const
{   
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgePopulation( collision.GetPopulation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulation
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KS_KnowledgeGroupQuerier::GetKnowledgePopulation( const DEC_Knowledge_PopulationPerception& perception ) const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgePopulation( perception.GetPopulationPerceived() );
}
