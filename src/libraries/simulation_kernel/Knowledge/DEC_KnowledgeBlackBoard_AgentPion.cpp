// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_AgentPion.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_AgentPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"

#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgeObjectPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "DEC_KS_ObjectInteraction.h"
#include "DEC_KS_PopulationInteraction.h"
#include "DEC_KS_Fire.h"
#include "DEC_KS_Perception.h"
#include "DEC_Knowledge_RapForLocal.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_Population.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/MIL_Army.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Decision/DEC_Tools.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard_AgentPion, "DEC_KnowledgeBlackBoard_AgentPion" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_AgentPion::DEC_KnowledgeBlackBoard_AgentPion( MIL_AgentPion& pion )
    : pPion_                                    ( &pion )
    , pKnowledgeAgentPerceptionContainer_       ( new DEC_BlackBoard_CanContainKnowledgeAgentPerception     () )
    , pKnowledgeObjectCollisionContainer_       ( new DEC_BlackBoard_CanContainKnowledgeObjectCollision     () )
    , pKnowledgeObjectPerceptionContainer_      ( new DEC_BlackBoard_CanContainKnowledgeObjectPerception    () )
    , pKnowledgePopulationPerceptionContainer_  ( new DEC_BlackBoard_CanContainKnowledgePopulationPerception() )
    , pKnowledgePopulationCollisionContainer_   ( new DEC_BlackBoard_CanContainKnowledgePopulationCollision () )
    , pKnowledgeRapForLocal_                    ( new DEC_Knowledge_RapForLocal( pion ) )
    , pKsObjectInteraction_                     ( new DEC_KS_ObjectInteraction    ( *this ) )
    , pKsPopulationInteraction_                 ( new DEC_KS_PopulationInteraction( *this ) )
    , pKsFire_                                  ( new DEC_KS_Fire                 ( *this ) )
    , pKsPerception_                            ( new DEC_KS_Perception           ( *this ) )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_AgentPion::DEC_KnowledgeBlackBoard_AgentPion()
    : pPion_                                    ( 0 )
    , pKnowledgeAgentPerceptionContainer_       ( 0 )
    , pKnowledgeObjectCollisionContainer_       ( 0 )
    , pKnowledgeObjectPerceptionContainer_      ( 0 )
    , pKnowledgePopulationPerceptionContainer_  ( 0 )
    , pKnowledgePopulationCollisionContainer_   ( 0 )
    , pKnowledgeRapForLocal_                    ( 0 )
    , pKsObjectInteraction_                     ( 0 )
    , pKsPopulationInteraction_                 ( 0 )
    , pKsFire_                                  ( 0 )
    , pKsPerception_                            ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_AgentPion::~DEC_KnowledgeBlackBoard_AgentPion()
{
    delete pKnowledgeAgentPerceptionContainer_;     
    delete pKnowledgeObjectCollisionContainer_;     
    delete pKnowledgeObjectPerceptionContainer_;    
    delete pKnowledgePopulationPerceptionContainer_;
    delete pKnowledgePopulationCollisionContainer_; 
    delete pKsObjectInteraction_;                   
    delete pKsPopulationInteraction_;               
    delete pKsFire_;                                
    delete pKsPerception_;                             
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_AgentPion::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeBlackBoard_AgentPion::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pPion_                                   
            & pKnowledgeAgentPerceptionContainer_      
            & pKnowledgeObjectCollisionContainer_      
            & pKnowledgeObjectPerceptionContainer_     
            & pKnowledgePopulationPerceptionContainer_ 
            & pKnowledgePopulationCollisionContainer_  
            & pKsObjectInteraction_                    
            & pKsPopulationInteraction_                
            & pKsFire_                                 
            & pKsPerception_
            & pKnowledgeRapForLocal_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::SendFullState() const
{
//    pKnowledgeObjectCollisionContainer_;
//    knowledgeRapForLocal_

    pKnowledgeAgentPerceptionContainer_     ->ApplyOnKnowledgesAgentPerception     ( std::mem_fun_ref( & DEC_Knowledge_AgentPerception     ::SendStateToNewClient ) );
    pKnowledgeObjectPerceptionContainer_    ->ApplyOnKnowledgesObjectPerception    ( std::mem_fun_ref( & DEC_Knowledge_ObjectPerception    ::SendStateToNewClient ) );
    pKnowledgePopulationPerceptionContainer_->ApplyOnKnowledgesPopulationPerception( std::mem_fun_ref( & DEC_Knowledge_PopulationPerception::SendStateToNewClient ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::SendChangedState() const
{
//    pKnowledgeObjectCollisionContainer_;
//    knowledgeRapForLocal_

    pKnowledgeAgentPerceptionContainer_     ->ApplyOnKnowledgesAgentPerception     ( std::mem_fun_ref( & DEC_Knowledge_AgentPerception     ::UpdateOnNetwork ) );
    pKnowledgeObjectPerceptionContainer_    ->ApplyOnKnowledgesObjectPerception    ( std::mem_fun_ref( & DEC_Knowledge_ObjectPerception    ::UpdateOnNetwork ) );
    pKnowledgePopulationPerceptionContainer_->ApplyOnKnowledgesPopulationPerception( std::mem_fun_ref( & DEC_Knowledge_PopulationPerception::UpdateOnNetwork ) );
}


// =============================================================================
// QUERIES
// =============================================================================

// -----------------------------------------------------------------------------
// Agents
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentPerceptionContainer_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pKnowledgeAgentPerceptionContainer_->GetKnowledgeAgentPerception( agent );
    if ( pKnowledge )
        return pKnowledge->GetCurrentPerceptionLevel() == PHY_PerceptionLevel::identified_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsPerceived
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsPerceived( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentPerceptionContainer_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pKnowledgeAgentPerceptionContainer_->GetKnowledgeAgentPerception( agent );
    if( pKnowledge )
        return pKnowledge->IsPerceived();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::WasPerceived
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::WasPerceived( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentPerceptionContainer_ );
    
    DEC_Knowledge_AgentPerception* pKnowledge = pKnowledgeAgentPerceptionContainer_->GetKnowledgeAgentPerception( agent );
    if( pKnowledge )
        return pKnowledge->GetPreviousPerceptionLevel() > PHY_PerceptionLevel::notSeen_;
    return false;
}

namespace {
    template< class T >
    class sLivingEnemiesPerceivedInsertor
    {
    public:
        sLivingEnemiesPerceivedInsertor( const MIL_AgentPion& pion, T_KnowledgeAgentDiaIDVector& container, const T* pZone = 0 )
            : pContainer_( &container )
            , pPion_     ( &pion )
            , pArmy_     ( &pion.GetArmy() )
            , pZone_     ( pZone )
        {
        }

        void operator() ( DEC_Knowledge_AgentPerception& knowledge )
        {
            if( !knowledge.IsAvailable() ) // Record mode
                return;
                
            DEC_Knowledge_Agent* pKnowledge = pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgent( knowledge );
            assert( pKnowledge );

            if(    knowledge.GetAgentPerceived() == *pPion_ 
                || pKnowledge->IsDead()
                || pKnowledge->IsAnEnemy( *pArmy_ ) != eTristate_True
                || pKnowledge->IsRefugee()
                || pKnowledge->IsSurrendered() )
                return;

            if( pZone_ && !pZone_->IsInside( pKnowledge->GetPosition() ) )
                return;

            pContainer_->push_back( (void*)pKnowledge->GetID() );            
        }

    private:
              T_KnowledgeAgentDiaIDVector* pContainer_;
        const MIL_AgentPion*               pPion_;
        const MIL_Army_ABC*                pArmy_;
        const T*                           pZone_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceived
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceived( T_KnowledgeObjectDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pKnowledgeAgentPerceptionContainer_ );

    container.clear();
    sLivingEnemiesPerceivedInsertor< TER_Polygon > functor( *pPion_, container );
    pKnowledgeAgentPerceptionContainer_->ApplyOnKnowledgesAgentPerception( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceivedInZone
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceivedInZone( T_KnowledgeObjectDiaIDVector& container, const TER_Polygon& zone ) const
{
    assert( pPion_ );
    assert( pKnowledgeAgentPerceptionContainer_ );

    container.clear();
    sLivingEnemiesPerceivedInsertor< TER_Polygon > functor( *pPion_, container, &zone );
    pKnowledgeAgentPerceptionContainer_->ApplyOnKnowledgesAgentPerception( functor );           
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceivedInZone
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetLivingEnemiesPerceivedInZone( T_KnowledgeObjectDiaIDVector& container, const TER_Localisation& zone ) const
{
    assert( pPion_ );
    assert( pKnowledgeAgentPerceptionContainer_ );

    container.clear();
    sLivingEnemiesPerceivedInsertor< TER_Localisation > functor( *pPion_, container, &zone );
    pKnowledgeAgentPerceptionContainer_->ApplyOnKnowledgesAgentPerception( functor );           
}

namespace {
    class sAgentsAttackingInsertor
    {
    public:
        sAgentsAttackingInsertor( const MIL_AgentPion& pion, T_KnowledgeAgentDiaIDVector& container )
            : pContainer_( &container )
            , pPion_     ( &pion )
        {
        }

        void operator() ( DEC_Knowledge_AgentPerception& knowledge )
        {
            if( !knowledge.IsAttacker() || !knowledge.IsAvailable() ) // Record mode
                return;

            DEC_Knowledge_Agent* pKnowledge = pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgent( knowledge );
            assert( pKnowledge );
            pContainer_->push_back( (void*)pKnowledge->GetID() );
        }

    private:
            T_KnowledgeAgentDiaIDVector* pContainer_;
        const MIL_AgentPion*               pPion_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetAgentsAttacking
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetAgentsAttacking( T_KnowledgeAgentDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pKnowledgeAgentPerceptionContainer_ );

    container.clear();
    sAgentsAttackingInsertor functor( *pPion_, container );
    pKnowledgeAgentPerceptionContainer_->ApplyOnKnowledgesAgentPerception( functor );              
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetRapForLocalValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
MT_Float DEC_KnowledgeBlackBoard_AgentPion::GetRapForLocalValue() const
{
    assert( pKnowledgeRapForLocal_ );
    return pKnowledgeRapForLocal_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetDangerousEnemies
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentDiaIDVector& DEC_KnowledgeBlackBoard_AgentPion::GetDangerousEnemies() const
{
    assert( pKnowledgeRapForLocal_ );
    return pKnowledgeRapForLocal_->GetDangerousEnemiesIDs();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetDangerousEnemies
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetDangerousEnemiesInZone( T_KnowledgeAgentDiaIDVector& container, const TER_Localisation& zone ) const
{    
    container.clear();

    assert( pKnowledgeRapForLocal_ );
    const T_KnowledgeAgentVector& knowledges = pKnowledgeRapForLocal_->GetDangerousEnemies();
    for( CIT_KnowledgeAgentVector it = knowledges.begin(); it != knowledges.end(); ++it )
    {
        const DEC_Knowledge_Agent& knowledge = **it;
        if( zone.IsInside( knowledge.GetPosition() ) )
            container.push_back( (void*)knowledge.GetID() );
    }
}

// -----------------------------------------------------------------------------
// Objects
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsIdentified( const MIL_Object_ABC& object ) const
{
    assert( pKnowledgeObjectPerceptionContainer_ );
    
    DEC_Knowledge_ObjectPerception* pKnowledge = pKnowledgeObjectPerceptionContainer_->GetKnowledgeObjectPerception( object );
    if( pKnowledge )
        return pKnowledge->GetCurrentPerceptionLevel() == PHY_PerceptionLevel::identified_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsPerceived
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsPerceived( const MIL_Object_ABC& object ) const
{
    assert( pKnowledgeObjectPerceptionContainer_ );
    
    DEC_Knowledge_ObjectPerception* pKnowledge = pKnowledgeObjectPerceptionContainer_->GetKnowledgeObjectPerception( object );
    if( pKnowledge )
        return pKnowledge->IsPerceived();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetObjectsColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetObjectsColliding( T_KnowledgeObjectCollisionVector& container ) const
{
    assert( pKnowledgeObjectCollisionContainer_ );
    pKnowledgeObjectCollisionContainer_->GetKnowledgesObjectCollision( container );    
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetObjectsColliding( T_KnowledgeObjectDiaIDVector& container ) const
{
    assert( pPion_ );
    
    T_KnowledgeObjectCollisionVector objectsColliding;
    GetObjectsColliding( objectsColliding );

    container.clear(); container.reserve( objectsColliding.size() );
    for ( CIT_KnowledgeObjectCollisionVector itObjectColliding = objectsColliding.begin(); itObjectColliding != objectsColliding.end(); ++itObjectColliding )
    {
        DEC_Knowledge_Object* pKnowledge = pPion_->GetArmy().GetKnowledge().GetKnowledgeObject( **itObjectColliding );
        assert( pKnowledge );
        container.push_back( (void*)( pKnowledge->GetID() ) );
    }
}

// -----------------------------------------------------------------------------
// Populations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    assert( pKnowledgePopulationPerceptionContainer_ );
    
    DEC_Knowledge_PopulationPerception* pKnowledge = pKnowledgePopulationPerceptionContainer_->GetKnowledgePopulationPerception( concentration.GetPopulation() );
    if( pKnowledge )
        return pKnowledge->IsIdentified( concentration );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::IsPerceived
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_AgentPion::IsPerceived( const MIL_Population& population ) const
{
    assert( pKnowledgePopulationPerceptionContainer_ );

    DEC_Knowledge_PopulationPerception* pKnowledge = pKnowledgePopulationPerceptionContainer_->GetKnowledgePopulationPerception( population );
    if( pKnowledge )
        return pKnowledge->IsPerceived();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsColliding( T_KnowledgePopulationCollisionVector& container ) const
{
    assert( pKnowledgePopulationCollisionContainer_ );
    pKnowledgePopulationCollisionContainer_->GetKnowledgesPopulationCollision( container );  
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsColliding( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pPion_ );
    T_KnowledgePopulationCollisionVector populationsColliding;
    GetPopulationsColliding( populationsColliding );

    container.clear();
    container.reserve( populationsColliding.size() );
    for ( CIT_KnowledgePopulationCollisionVector it = populationsColliding.begin(); it != populationsColliding.end(); ++it )
    {
        DEC_Knowledge_Population* pKnowledge = pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulation( **it );
        assert( pKnowledge );
        container.push_back( (void*)( pKnowledge->GetID() ) );
    }
}
  
namespace {
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

            DEC_Knowledge_Population* pKnowledge = pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulation( knowledge );
            assert( pKnowledge );
            pContainer_->push_back( (void*)pKnowledge->GetID() );
        }

    private:
            T_KnowledgePopulationDiaIDVector* pContainer_;
        const MIL_AgentPion*                    pPion_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsAttacking
// Created: NLD 2004-03-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_AgentPion::GetPopulationsAttacking( T_KnowledgePopulationDiaIDVector& container ) const
{
    assert( pPion_ );
    assert( pKnowledgePopulationPerceptionContainer_ );

    container.clear();
    sPopulationAttackingInsertor functor( *pPion_, container );
    pKnowledgePopulationPerceptionContainer_->ApplyOnKnowledgesPopulationPerception( functor );              
}

// =============================================================================
// RESOLVER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent( const ASN1T_UnitKnowledge& asn ) const
{
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent( const DIA_Variable_ABC& dia ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( dia ) );
    uint nDiaID = (uint)dia.ToPtr();
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( nDiaID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeAgent( uint nID ) const
{
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const
{
    return pPion_->GetKnowledgeGroup().GetArmy().GetKnowledge().GetKnowledgeObjectFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject( const DIA_Variable_ABC& dia ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( dia ) );
    uint nDiaID = (uint)dia.ToPtr();
    return pPion_->GetKnowledgeGroup().GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nDiaID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgeObject( uint nID ) const
{
    return pPion_->GetKnowledgeGroup().GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const
{
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation( const DIA_Variable_ABC& dia ) const
{
    assert( DEC_Tools::CheckTypeConnaissancePopulation( dia ) );
    uint nDiaID = (uint)dia.ToPtr();
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nDiaID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_AgentPion::ResolveKnowledgePopulation( uint nID ) const
{
    return pPion_->GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nID );
}