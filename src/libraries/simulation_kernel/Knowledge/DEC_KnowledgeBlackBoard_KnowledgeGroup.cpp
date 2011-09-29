// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 18:48 $
// $Revision: 2 $
// $Workfile: DEC_KnowledgeBlackBoard_KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_KS_KnowledgeSynthetizer.h"
#include "DEC_KS_Sharing.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KnowledgeBlackBoard_KnowledgeGroup )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_KnowledgeGroup::DEC_KnowledgeBlackBoard_KnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
    : pKnowledgeGroup_              ( &knowledgeGroup )
    , pKnowledgeAgentContainer_     ( new DEC_BlackBoard_CanContainKnowledgeAgent( knowledgeGroup ) )
    , pKnowledgePopulationContainer_( new DEC_BlackBoard_CanContainKnowledgePopulation() )
    , pKnowledgeObjectContainer_    ( 0 )
    , pKsKnowledgeSynthetizer_      ( new DEC_KS_KnowledgeSynthetizer( *this ) )
    , pKsSharing_                   ( new DEC_KS_Sharing( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_KnowledgeGroup::DEC_KnowledgeBlackBoard_KnowledgeGroup()
    : pKnowledgeGroup_                  ( 0 )
    , pKnowledgeAgentContainer_         ( 0 )
    , pKnowledgePopulationContainer_    ( 0 )
    , pKnowledgeObjectContainer_        ( 0 )
    , pKsKnowledgeSynthetizer_          ( 0 )
    , pKsSharing_                       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_KnowledgeGroup::~DEC_KnowledgeBlackBoard_KnowledgeGroup()
{
    delete pKnowledgeAgentContainer_;
    delete pKnowledgePopulationContainer_;
    delete pKnowledgeObjectContainer_;
    delete pKsKnowledgeSynthetizer_;
    delete pKsSharing_;
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_KnowledgeGroup::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_KnowledgeBlackBoard_KnowledgeGroup::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pKnowledgeGroup_
            & pKnowledgeAgentContainer_
            & pKnowledgePopulationContainer_
            & pKnowledgeObjectContainer_
            & pKsKnowledgeSynthetizer_
            & pKsSharing_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::SendFullState( unsigned int /*nCtx*/ ) const
{
    boost::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( &DEC_Knowledge_Agent::SendStateToNewClient, _1 );
    pKnowledgeAgentContainer_->ApplyOnKnowledgesAgent( agentFunctor );
    boost::function< void( DEC_Knowledge_Population& ) > populationFunctor = boost::bind( &DEC_Knowledge_Population::SendStateToNewClient, _1 );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( populationFunctor );
    if( pKnowledgeObjectContainer_ )
    {
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > objectFunctor = boost::bind( &DEC_Knowledge_Object::SendStateToNewClient, _1 );
        pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( objectFunctor );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::Update
// Created: LDC 2011-08-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::Update( int currentTimeStep )
{
    if( pKnowledgeObjectContainer_ )
    {
        std::mem_fun_ref_t< void, DEC_Knowledge_Object > objectFunctor = std::mem_fun_ref( &DEC_Knowledge_Object::Prepare );
        pKnowledgeObjectContainer_->ApplyOnKnowledgesObjectRef( objectFunctor );
    }
    DEC_KnowledgeBlackBoard_ABC::Update( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::SendChangedState() const
{
    boost::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( &DEC_Knowledge_Agent::UpdateOnNetwork, _1 );
    pKnowledgeAgentContainer_->ApplyOnKnowledgesAgent( agentFunctor );
    boost::function< void( DEC_Knowledge_Population& ) > populationFunctor = boost::bind( &DEC_Knowledge_Population::UpdateOnNetwork, _1 );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( populationFunctor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::SendObjectChangedState
// Created: LDC 2011-08-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::SendObjectChangedState() const
{
    if( pKnowledgeObjectContainer_ )
    {
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > objectFunctor = boost::bind( &DEC_Knowledge_Object::UpdateOnNetwork, _1 );
        pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( objectFunctor );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::IsKnown
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_KnowledgeGroup::IsKnown( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->HasKnowledgeAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgentFromID
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgentFromID( unsigned int nID ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent( const DEC_Knowledge_AgentPerception& perception ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgent( perception.GetAgentPerceived() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetDetectedAgentsInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& detected = pKnowledgeAgentContainer_->GetDetected();
    for( CIT_KnowledgeAgentVector it = detected.begin(); it != detected.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetDetectedAgentsInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetDetectedAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& detected = pKnowledgeAgentContainer_->GetDetected();
    for( CIT_KnowledgeAgentVector it = detected.begin(); it != detected.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetEnemyAgentsInZone
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetEnemyAgentsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& enemies = pKnowledgeAgentContainer_->GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetFriends
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetFriends() const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetFriends();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetFriendsInZone
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetFriendsInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& friends = GetFriends();
    for( CIT_KnowledgeAgentVector it = friends.begin(); it != friends.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetEnemies
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetEnemies() const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetEnemies();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInZone
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Localisation& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& enemies = GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( !knowledge->IsDead() && zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInZone
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInZone( T_ConstKnowledgeAgentVector& container, const TER_Polygon& zone ) const
{
    container.clear();
    const T_KnowledgeAgentVector& enemies = GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( !knowledge->IsDead() && zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const
{
    container.clear();
    const T_KnowledgeAgentVector& enemies = GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( !knowledge->IsDead() && center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_KnowledgeGroup::EnemyPresenceInCircle( const MT_Vector2D& center, double rRadius ) const
{
    const T_KnowledgeAgentVector& enemies = GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        const DEC_Knowledge_Agent& knowledge = **it;
        if( center.Distance( knowledge.GetPosition() ) <= rRadius )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetRefugeesInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetRefugeesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const
{
    assert( pKnowledgeAgentContainer_ );
    container.clear();
    const T_KnowledgeAgentVector& refugees = pKnowledgeAgentContainer_->GetRefugees();
    for( CIT_KnowledgeAgentVector it = refugees.begin(); it != refugees.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetTerroristsInCircle
// Created: DDA 2006-08-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetTerroristsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const
{
    assert( pKnowledgeAgentContainer_ );
    container.clear();
    const T_KnowledgeAgentVector& terrorists = pKnowledgeAgentContainer_->GetTerrorists();
    for( CIT_KnowledgeAgentVector it = terrorists.begin(); it != terrorists.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetSurrenderedAgentsInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetSurrenderedAgentsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, double rRadius ) const
{
    assert( pKnowledgeAgentContainer_ );
    container.clear();
    const T_KnowledgeAgentVector& surrenderedAgents = pKnowledgeAgentContainer_->GetSurrenderedAgents();
    for( CIT_KnowledgeAgentVector it = surrenderedAgents.begin(); it != surrenderedAgents.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = *it;
        if( center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationFromID
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationFromID( unsigned int nID ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulationFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation( const MIL_Population& population ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulation( population );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation( const DEC_Knowledge_PopulationCollision& collision ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulation( collision.GetPopulation() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulation( const DEC_Knowledge_PopulationPerception& perception ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulation( perception.GetPopulationPerceived() );
}

namespace
{
    class sDIAInserter
    {
    public:
        sDIAInserter( T_KnowledgePopulationDiaIDVector& container ) : pContainer_( &container ) {}
        void operator() ( DEC_Knowledge_Population& knowledge )
        {
            pContainer_->push_back( knowledge.GetID() );
        }
    private:
        T_KnowledgePopulationDiaIDVector* pContainer_;
    };

    class sInserter
    {
    public:
        sInserter( T_KnowledgePopulationVector& container ) : pContainer_( &container ) {}
        void operator() ( DEC_Knowledge_Population& knowledge )
        {
            pContainer_->push_back( &knowledge );
        }
    private:
        T_KnowledgePopulationVector* pContainer_;
    };

}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetPopulations( T_KnowledgePopulationDiaIDVector& container ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sDIAInserter functor( container );
    assert( pKnowledgePopulationContainer_ );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgesPopulation
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetPopulations( T_KnowledgePopulationVector& container ) const
{
    container.clear();
    assert( pKnowledgeGroup_ );
    sInserter functor( container );
    assert( pKnowledgePopulationContainer_ );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( functor );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::TranslateKnowledges
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::TranslateKnowledges( const T_ConstKnowledgeAgentVector& sourceKnowledges, const MIL_KnowledgeGroup& sourceKnowledgeGroup, T_ConstKnowledgeAgentVector& translatedKnowledges ) const
{
    assert( pKnowledgeGroup_ );
    if( *pKnowledgeGroup_ == sourceKnowledgeGroup )
        translatedKnowledges = sourceKnowledges;
    else
    {
        translatedKnowledges.clear();
        for( CIT_ConstKnowledgeAgentVector it = sourceKnowledges.begin(); it != sourceKnowledges.end(); ++it )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > pSourceKnowledge = *it;
            boost::shared_ptr< DEC_Knowledge_Agent > pTranslatedKnowledge = pKnowledgeAgentContainer_->GetKnowledgeAgent( pSourceKnowledge->GetAgentKnown() );
            if( pTranslatedKnowledge.get() && pTranslatedKnowledge->IsValid() )
                translatedKnowledges.push_back( pTranslatedKnowledge );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent( const sword::UnitKnowledgeId& asn ) const
{
    return GetKnowledgeAgentFromID( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent( unsigned int nID ) const
{
    return GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const
{
    return ResolveKnowledgeObject( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject( unsigned int nID ) const
{
    if( pKnowledgeObjectContainer_ )
        return pKnowledgeObjectContainer_->GetKnowledgeObjectFromID( nID );
    return pKnowledgeGroup_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject( const MIL_Object_ABC& object ) const
{
    if( pKnowledgeObjectContainer_ )
        return pKnowledgeObjectContainer_->GetKnowledgeObject( object );
    return pKnowledgeGroup_->GetArmy().GetKnowledge().GetKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObjectByObjectID
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObjectByObjectID( unsigned int nID ) const
{
    if( pKnowledgeObjectContainer_ )
        return pKnowledgeObjectContainer_->GetKnowledgeObjectFromObjectID( nID );
    return pKnowledgeGroup_->GetArmy().GetKnowledge().GetKnowledgeObjectFromObjectID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& asn ) const
{
    return GetKnowledgePopulationFromID( asn.id() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation
// Created: LGY 2011-07-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation( const MIL_Population& population ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulation( population );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation( unsigned int nID ) const
{
    return GetKnowledgePopulationFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKsSharing
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_Sharing& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKsSharing() const
{
    assert( pKsSharing_ );
    return *pKsSharing_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeGroup
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgentContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgentContainer() const
{
    assert( pKnowledgeAgentContainer_ );
    return *pKnowledgeAgentContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgePopulation& DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationContainer() const
{
    assert( pKnowledgePopulationContainer_ );
    return *pKnowledgePopulationContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeObjectContainer
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeObject* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeObjectContainer() const
{
    return pKnowledgeObjectContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::Jam
// Created: LDC 2010-04-06
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::Jam()
{
    pKnowledgeObjectContainer_ = new DEC_BlackBoard_CanContainKnowledgeObject( pKnowledgeGroup_->GetArmy(), pKnowledgeGroup_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgeObject
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgeObject( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown )
{
    return pKnowledgeObjectContainer_->CreateKnowledgeObject( teamKnowing, objectKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ApplyOnKnowledgesPerception
// Created: FDS 2010-04-08
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::ApplyOnKnowledgesPerception( int currentTimeStep )
{
    GetKnowledgeGroup().ApplyOnKnowledgesPerception( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgeAgent
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& perceived )
{
    return GetKnowledgeAgentContainer().CreateKnowledgeAgent( knowledgeGroup, perceived );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgePopulation
// Created: FDS 2010-04-12
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_KnowledgeBlackBoard_KnowledgeGroup::CreateKnowledgePopulation( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& perceived )
{
    return GetKnowledgePopulationContainer().CreateKnowledgePopulation( knowledgeGroup, perceived );
}

namespace
{
    class UpdateRelevanceHelper
    {
    public:
        UpdateRelevanceHelper( DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer )
            : pKnowledgeObjectContainer_( pKnowledgeObjectContainer ) {}
        void operator() ( boost::shared_ptr< DEC_Knowledge_Object > knowledge )
        {
            MIL_Object_ABC* knownObject = knowledge->GetObjectKnown();
            knowledge->UpdateRelevance();
            if( pKnowledgeObjectContainer_&& knownObject && !knowledge->GetObjectKnown() )
                pKnowledgeObjectContainer_->NotifyKnowledgeObjectDissociatedFromRealObject( *knownObject, *knowledge );
        }
    private:
        DEC_BlackBoard_CanContainKnowledgeObject* pKnowledgeObjectContainer_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::UpdateKnowledgeObjectContainer
// Created: SBO 2010-05-19
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::UpdateKnowledgeObjectContainer()
{
    if( pKnowledgeObjectContainer_ )
    {
        UpdateRelevanceHelper visitor( pKnowledgeObjectContainer_ );
        pKnowledgeObjectContainer_->ApplyOnKnowledgesObject( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    pKnowledgeAgentContainer_->Accept( visitor );
    pKnowledgePopulationContainer_->Accept( visitor );
    if( pKnowledgeObjectContainer_ )
        pKnowledgeObjectContainer_->Accept( visitor );
}
