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
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"  
#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_KS_AgentKnowledgeSynthetizer.h"
#include "DEC_KS_PopulationKnowledgeSynthetizer.h"
#include "DEC_KS_Sharing.h"
#include "MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_Army.h"

BOOST_CLASS_EXPORT_GUID( DEC_KnowledgeBlackBoard_KnowledgeGroup, "DEC_KnowledgeBlackBoard_KnowledgeGroup" )

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KnowledgeBlackBoard_KnowledgeGroup::DEC_KnowledgeBlackBoard_KnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
    : pKnowledgeGroup_                  ( &knowledgeGroup )
    , pKnowledgeAgentContainer_         ( new DEC_BlackBoard_CanContainKnowledgeAgent     ( knowledgeGroup ) )
    , pKnowledgePopulationContainer_    ( new DEC_BlackBoard_CanContainKnowledgePopulation() )
    , pKsAgentKnowledgeSynthetizer_     ( new DEC_KS_AgentKnowledgeSynthetizer            ( *this ) )      
    , pKsPopulationKnowledgeSynthetizer_( new DEC_KS_PopulationKnowledgeSynthetizer       ( *this ) )
    , pKsSharing_                       ( new DEC_KS_Sharing                              ( *this ) )
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
    , pKsAgentKnowledgeSynthetizer_     ( 0 )      
    , pKsPopulationKnowledgeSynthetizer_( 0 )
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
    delete pKsAgentKnowledgeSynthetizer_;     
    delete pKsPopulationKnowledgeSynthetizer_;
    delete pKsSharing_;                       
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_KnowledgeBlackBoard_KnowledgeGroup::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_KnowledgeBlackBoard_KnowledgeGroup::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_KnowledgeBlackBoard_ABC >( *this )
            & pKnowledgeGroup_                  
            & pKnowledgeAgentContainer_         
            & pKnowledgePopulationContainer_    
            & pKsAgentKnowledgeSynthetizer_     
            & pKsPopulationKnowledgeSynthetizer_
            & pKsSharing_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::SendFullState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::SendFullState() const
{
    pKnowledgeAgentContainer_     ->ApplyOnKnowledgesAgent     ( std::mem_fun_ref( & DEC_Knowledge_Agent     ::SendStateToNewClient ) );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( std::mem_fun_ref( & DEC_Knowledge_Population::SendStateToNewClient ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::SendChangedState
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::SendChangedState() const
{
    pKnowledgeAgentContainer_     ->ApplyOnKnowledgesAgent     ( std::mem_fun_ref( & DEC_Knowledge_Agent     ::UpdateOnNetwork ) );
    pKnowledgePopulationContainer_->ApplyOnKnowledgesPopulation( std::mem_fun_ref( & DEC_Knowledge_Population::UpdateOnNetwork ) );
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
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgentFromID( uint nID ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgeAgent( const DEC_Knowledge_AgentPerception& perception ) const
{
    assert( pKnowledgeAgentContainer_ );
    return pKnowledgeAgentContainer_->GetKnowledgeAgent( perception.GetAgentPerceived() );
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
        const DEC_Knowledge_Agent* knowledge = *it;
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
        const DEC_Knowledge_Agent& knowledge = **it;
        if( zone.IsInside( knowledge.GetPosition() ) )
            container.push_back( &knowledge );
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
        const DEC_Knowledge_Agent& knowledge = **it;
        if( zone.IsInside( knowledge.GetPosition() ) )
            container.push_back( &knowledge );
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
        const DEC_Knowledge_Agent* knowledge = *it;
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
        const DEC_Knowledge_Agent* knowledge = *it;
        if( !knowledge->IsDead() && zone.IsInside( knowledge->GetPosition() ) )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetLivingEnemiesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const
{
    container.clear();
    const T_KnowledgeAgentVector& enemies = GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); ++it )
    {
        const DEC_Knowledge_Agent* knowledge = *it;
        if( !knowledge->IsDead() && center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeBlackBoard_KnowledgeGroup::EnemyPresenceInCircle( const MT_Vector2D& center, MT_Float rRadius ) const
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
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetRefugeesInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const
{
    assert( pKnowledgeAgentContainer_ );
    container.clear();
    const T_KnowledgeAgentVector& refugees = pKnowledgeAgentContainer_->GetRefugees();
    for( CIT_KnowledgeAgentVector it = refugees.begin(); it != refugees.end(); ++it )
    {
        const DEC_Knowledge_Agent* knowledge = *it;
        if( center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetSurrenderedAgentsInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
void DEC_KnowledgeBlackBoard_KnowledgeGroup::GetSurrenderedAgentsInCircle( T_ConstKnowledgeAgentVector& container, const MT_Vector2D& center, MT_Float rRadius ) const
{
    assert( pKnowledgeAgentContainer_ );
    container.clear();
    const T_KnowledgeAgentVector& surrenderedAgents = pKnowledgeAgentContainer_->GetSurrenderedAgents();
    for( CIT_KnowledgeAgentVector it = surrenderedAgents.begin(); it != surrenderedAgents.end(); ++it )
    {
        const DEC_Knowledge_Agent* knowledge = *it;
        if( center.Distance( knowledge->GetPosition() ) <= rRadius )
            container.push_back( knowledge );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationFromID
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::GetKnowledgePopulationFromID( uint nID ) const
{
    assert( pKnowledgePopulationContainer_ );
    return pKnowledgePopulationContainer_->GetKnowledgePopulationFromID( nID );
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
            const DEC_Knowledge_Agent* pSourceKnowledge = sourceKnowledgeGroup.GetKnowledge().GetKnowledgeAgentFromID( (uint)*it );
            assert( pSourceKnowledge );

            const DEC_Knowledge_Agent* pTranslatedKnowledge = pKnowledgeAgentContainer_->GetKnowledgeAgent( pSourceKnowledge->GetAgentKnown() );
            if( pTranslatedKnowledge )
                translatedKnowledges.push_back( pTranslatedKnowledge );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent( const ASN1T_UnitKnowledge& asn ) const
{
    return GetKnowledgeAgentFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeAgent( uint nID ) const
{
    return GetKnowledgeAgentFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject( const ASN1T_ObjectKnowledge& asn ) const
{
    return pKnowledgeGroup_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Object* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgeObject( uint nID ) const
{
    return pKnowledgeGroup_->GetArmy().GetKnowledge().GetKnowledgeObjectFromID( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation( const ASN1T_PopulationKnowledge& asn ) const
{
    return GetKnowledgePopulationFromID( asn );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
DEC_Knowledge_Population* DEC_KnowledgeBlackBoard_KnowledgeGroup::ResolveKnowledgePopulation( uint nID ) const
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
