// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AgentQuerier.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 15:58 $
// $Revision: 9 $
// $Workfile: DEC_KS_AgentQuerier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_AgentQuerier.h"

#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_RapForLocal.h"
#include "DEC_Knowledge_RapForGLobal.h"
#include "DEC_KS_ArmyQuerier.h"
#include "DEC_KS_KnowledgeGroupQuerier.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/MIL_Army.h"

#include "MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentQuerier::DEC_KS_AgentQuerier( DEC_KnowledgeBlackBoard& blackBoard, const MIL_AgentPion& pion )
    : DEC_KnowledgeSource_ABC( blackBoard )
    , pPion_                 ( &pion      )
{
        
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentQuerier::~DEC_KS_AgentQuerier()
{
    
}

// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsKnown( const MIL_Agent_ABC& agent ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledgeGroup().GetKSQuerier().IsKnown( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsKnown( const MIL_RealObject_ABC& object ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKSQuerier().IsKnown( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( agent );
    if ( pKnowledge )
        return pKnowledge->GetCurrentPerceptionLevel() == PHY_PerceptionLevel::identified_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::WasPerceived
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::WasPerceived( const MIL_Agent_ABC& agent ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( agent );
    if( pKnowledge )
        return pKnowledge->GetPreviousPerceptionLevel() > PHY_PerceptionLevel::notSeen_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsIdentified( const MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerception( object );
    if( pKnowledge )
        return pKnowledge->GetCurrentPerceptionLevel() == PHY_PerceptionLevel::identified_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsPerceived
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsPerceived( const MIL_Agent_ABC& agent ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pBlackBoard_->GetKnowledgeAgentPerception( agent );
    if( pKnowledge )
        return pKnowledge->IsPerceived();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsPerceived
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsPerceived( const MIL_RealObject_ABC& object ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_ObjectPerception* pKnowledge = pBlackBoard_->GetKnowledgeObjectPerception( object );
    if( pKnowledge )
        return pKnowledge->IsPerceived();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_KS_AgentQuerier::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    assert( pBlackBoard_ );
    
    DEC_Knowledge_PopulationPerception* pKnowledge = pBlackBoard_->GetKnowledgePopulationPerception( concentration.GetPopulation() );
    if( pKnowledge )
        return pKnowledge->IsIdentified( concentration );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetKnowledgeAgentFromDiaID
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KS_AgentQuerier::GetKnowledgeAgentFromID( uint nID ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetKnowledgePopulationFromID
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KS_AgentQuerier::GetKnowledgePopulationFromID( uint nID ) const
{
    assert( pPion_ );
    return pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgePopulationFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetKnowledgeObjectFromID
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_AgentQuerier::GetKnowledgeObjectFromID( uint nID ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKSQuerier().GetKnowledgeObjectFromID( nID );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetKnowledgeObject
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_AgentQuerier::GetKnowledgeObject( MIL_RealObject_ABC& object ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKSQuerier().GetKnowledgeObject( object );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetAgentsAttacking
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
class sAgentAttackingInsertor
{
public:
    sAgentAttackingInsertor( const MIL_AgentPion& pion, T_KnowledgeAgentDiaIDVector& container )
        : pContainer_( &container )
        , pPion_     ( &pion )
    {
    }

    void operator() ( DEC_Knowledge_AgentPerception& knowledge )
    {
        if( !knowledge.IsAttacker() )
            return;

        DEC_Knowledge_Agent* pKnowledge = pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgeAgent( knowledge );
        assert( pKnowledge );
        pContainer_->push_back( (void*)pKnowledge->GetID() );
    }

private:
          T_KnowledgeAgentDiaIDVector* pContainer_;
    const MIL_AgentPion*               pPion_;
};
void DEC_KS_AgentQuerier::GetAgentsAttacking( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pBlackBoard_ );

    container.clear();
    sAgentAttackingInsertor functor( *pPion_, container );
    pBlackBoard_->ApplyOnKnowledgesAgentPerception( functor );              
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPerceivedAgents
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
class sPerceivedAgentInsertor
{
public:
    sPerceivedAgentInsertor( const MIL_AgentPion& pion, T_KnowledgeAgentDiaIDVector& container )
        : pContainer_( &container )
        , pPion_     ( &pion )
    {
    }

    void operator() ( DEC_Knowledge_AgentPerception& knowledge )
    {
        if( knowledge.GetAgentPerceived() == *pPion_ )
            return;

        DEC_Knowledge_Agent* pKnowledge = pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgeAgent( knowledge );
        assert( pKnowledge );
        pContainer_->push_back( (void*)pKnowledge->GetID() );
    }

private:
          T_KnowledgeAgentDiaIDVector* pContainer_;
    const MIL_AgentPion*               pPion_;
};

void DEC_KS_AgentQuerier::GetPerceivedAgents( T_KnowledgeObjectDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pBlackBoard_ );

    container.clear();
    sPerceivedAgentInsertor functor( *pPion_, container );
    pBlackBoard_->ApplyOnKnowledgesAgentPerception( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPotentiallyDangerousAgents
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetPotentiallyDangerousAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetPotentiallyDangerousAgents( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetMilitiaAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetMilitiaAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetMilitiaAgents( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetRefugeeAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetRefugeeAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetRefugeeAgents( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetSurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetSurrenderedAgents( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetSurrenderedAgents( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetEnemies( T_KnowledgeAgentVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetEnemies( container );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetFriendsInZone( T_KnowledgeAgentDiaIDVector& container, const TER_Localisation& zone ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetFriendsInZone( container, zone );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetObjects
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetObjects( T_KnowledgeObjectDiaIDVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    assert( pPion_ );
    pPion_->GetArmy().GetKSQuerier().GetObjects( container, filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetObjects
// Created: NLD 2005-05-09
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetObjects( T_KnowledgeObjectVector& container ) const
{
    assert( pPion_ );
    pPion_->GetArmy().GetKSQuerier().GetObjects( container );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetObjects
// Created: NLD 2004-04-09
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetObjects( T_KnowledgeObjectVector& container, const MIL_RealObjectTypeFilter& filter ) const
{
    assert( pPion_ );
    pPion_->GetArmy().GetKSQuerier().GetObjects( container, filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_AgentQuerier::GetClosestObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKSQuerier().GetClosestObject( vPos, filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetClosestFriendObject
// Created: NLD 2006-01-05
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KS_AgentQuerier::GetClosestFriendObject( const MT_Vector2D& vPos, const MIL_RealObjectTypeFilter& filter  ) const
{
    assert( pPion_ );
    return pPion_->GetArmy().GetKSQuerier().GetClosestFriendObject( vPos, filter );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetObjectsColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetObjectsColliding( T_KnowledgeObjectCollisionVector& container ) const
{
    assert( pBlackBoard_ );
    pBlackBoard_->GetKnowledgesObjectCollision( container );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetObjectsColliding( T_KnowledgeObjectDiaIDVector& container ) const
{
    assert( pPion_ );

    container.clear();
    T_KnowledgeObjectCollisionVector objectsColliding;
    GetObjectsColliding( objectsColliding );
    for ( CIT_KnowledgeObjectCollisionVector itObjectColliding = objectsColliding.begin(); itObjectColliding != objectsColliding.end(); ++itObjectColliding )
    {
        DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKSQuerier().GetKnowledgeObject( **itObjectColliding );
        assert( pKnowledge );
        container.push_back( (void*)( pKnowledge->GetID() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetPopulationsColliding( T_KnowledgePopulationCollisionVector& container ) const
{
    assert( pBlackBoard_ );
    pBlackBoard_->GetKnowledgesPopulationCollision( container );  
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetPopulationsColliding( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pPion_ );

    container.clear();
    T_KnowledgePopulationCollisionVector populationsColliding;
    GetPopulationsColliding( populationsColliding );
    for ( CIT_KnowledgePopulationCollisionVector it = populationsColliding.begin(); it != populationsColliding.end(); ++it )
    {
        DEC_Knowledge_Population* pKnowledge = pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgePopulation( **it );
        assert( pKnowledge );
        container.push_back( (void*)( pKnowledge->GetID() ) );
    }
}
    
// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPopulationsAttacking
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
class sPopulationAttackingInsertor
{
public:
    sPopulationAttackingInsertor( const MIL_AgentPion& pion, T_KnowledgePopulationDiaIDVector& container )
        : pContainer_( &container )
        , pPion_     ( &pion )
    {
    }

    void operator() ( DEC_Knowledge_PopulationPerception& knowledge )
    {
        if( !knowledge.IsAttacker() )
            return;

        DEC_Knowledge_Population* pKnowledge = pPion_->GetKnowledgeGroup().GetKSQuerier().GetKnowledgePopulation( knowledge );
        assert( pKnowledge );
        pContainer_->push_back( (void*)pKnowledge->GetID() );
    }

private:
          T_KnowledgePopulationDiaIDVector* pContainer_;
    const MIL_AgentPion*                    pPion_;
};
void DEC_KS_AgentQuerier::GetPopulationsAttacking( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pBlackBoard_ );

    container.clear();
    sPopulationAttackingInsertor functor( *pPion_, container );
    pBlackBoard_->ApplyOnKnowledgesPopulationPerception( functor );              
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KS_AgentQuerier::GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pPion_ );
    pPion_->GetKnowledgeGroup().GetKSQuerier().GetPopulations( container );
}

//------------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetKnowledgeRapForLocal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal& DEC_KS_AgentQuerier::GetKnowledgeRapForLocal() const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeRapForLocal();
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentQuerier::GetRapForLocalValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
MT_Float DEC_KS_AgentQuerier::GetRapForLocalValue() const
{
    assert( pBlackBoard_ );
    return pBlackBoard_->GetKnowledgeRapForLocal().GetValue();
}


