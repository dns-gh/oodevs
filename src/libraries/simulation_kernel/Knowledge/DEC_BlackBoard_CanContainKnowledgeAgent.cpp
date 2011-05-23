// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.cpp $
// $Author: Nld $
// $Modtime: 20/04/05 19:05 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgent.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_KnowledgeSource_ABC.h"
#include "DEC_Knowledge_Agent.h"
#include "MIL_AgentServer.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"
#include "MT_Tools/MT_ScipioException.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeAgent )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent constructor
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::DEC_BlackBoard_CanContainKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup )
    : pKnowledgeGroup_           ( &knowledgeGroup )
    , nLastCacheUpdateTick_      ( 0 )
    , detectedContainer_         ()
    , friendsContainer_          ()
    , enemiesContainer_          ()
    , militiasContainer_         ()
    , terroristsContainer_       ()
    , refugeesContainer_         ()
    , surrenderedAgentsContainer_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::DEC_BlackBoard_CanContainKnowledgeAgent()
    : pKnowledgeGroup_           ( 0 )
    , nLastCacheUpdateTick_      ( 0 )
    , detectedContainer_         ()
    , friendsContainer_          ()
    , enemiesContainer_          ()
    , militiasContainer_         ()
    , terroristsContainer_       ()
    , refugeesContainer_         ()
    , surrenderedAgentsContainer_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::~DEC_BlackBoard_CanContainKnowledgeAgent()
{
    while( !unitKnowledgeFromIDMap_.empty() )
        DestroyKnowledgeAgent( *unitKnowledgeFromIDMap_.begin()->second );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const unsigned int )
        {
            const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap::size_type size = map.size();
            file << size;
            for ( DEC_BlackBoard_CanContainKnowledgeAgent::CIT_KnowledgeAgentMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << *it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const unsigned int )
        {
            DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap::size_type nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC*       pAgent;
                boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge( new DEC_Knowledge_Agent() );

                file >> pAgent
                     >> *pKnowledge;

                map.insert( std::make_pair( pAgent, pKnowledge ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ )
         >> nLastCacheUpdateTick_;
    std::size_t size;
    file >> size;
    for( std::size_t i = 0; i < size; ++i )
    {
        MIL_Agent_ABC* agent;
        file >> agent;
        realAgentMap_[ agent ].reset( new DEC_Knowledge_Agent() );
        file >> *realAgentMap_[ agent ];
        unitKnowledgeFromIDMap_.insert( std::make_pair( realAgentMap_[ agent ]->GetID(), realAgentMap_[ agent ] ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pKnowledgeGroup_
         << nLastCacheUpdateTick_;
    std::size_t size = realAgentMap_.size();
    file << size;
    for( CIT_KnowledgeAgentMap it = realAgentMap_.begin(); it != realAgentMap_.end(); ++it )
    {
        MIL_Agent_ABC* agent = const_cast<MIL_Agent_ABC*>( it->first );
        file << agent;
        file << *it->second;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown )
{
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge = agentKnown.CreateKnowledge( knowledgeGroup );
    if( ! realAgentMap_.insert( std::make_pair( &agentKnown, knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    if( ! unitKnowledgeFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    return *knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    knowledge.Invalidate();
    if( realAgentMap_.erase( &knowledge.GetAgentKnown() ) < 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
    if( unitKnowledgeFromIDMap_.erase( knowledge.GetID() ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::UpdateQueriesCache
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::UpdateQueriesCache()
{
    if( nLastCacheUpdateTick_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
        return;

    nLastCacheUpdateTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();

    detectedContainer_         .clear();
    friendsContainer_          .clear();
    enemiesContainer_          .clear();
    militiasContainer_         .clear();
    terroristsContainer_       .clear();
    refugeesContainer_         .clear();
    surrenderedAgentsContainer_.clear();

    assert( pKnowledgeGroup_ );
    const MIL_Army_ABC& army = pKnowledgeGroup_->GetArmy();

    for( CIT_KnowledgeAgentMap itKnowledge = realAgentMap_.begin(); itKnowledge != realAgentMap_.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = itKnowledge->second;

        if( knowledge->IsRefugee() )
            refugeesContainer_.push_back( knowledge );
        else if( knowledge->IsSurrendered() )
        {
            if( !knowledge->GetArmy() || *knowledge->GetArmy() != army )
                surrenderedAgentsContainer_.push_back( knowledge );
        }
        else
        {
            if( !knowledge->GetArmy() )
            {
                if( army.IsAFriend( knowledge->GetAgentKnown().GetArmy() ) != eTristate_True )
                    detectedContainer_.push_back( knowledge );
            }
            else if( knowledge->IsAFriend( army ) == eTristate_True )
                friendsContainer_.push_back( knowledge );
            else if( knowledge->IsAnEnemy( army ) == eTristate_True )
                enemiesContainer_.push_back( knowledge );
            if( knowledge->IsMilitia() )
                militiasContainer_.push_back( knowledge );
            if( knowledge->IsTerrorist() )
                terroristsContainer_.push_back( knowledge );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const
{
    CIT_KnowledgeAgentMap it = realAgentMap_.find( &agentKnown );
    if( it == realAgentMap_.end() )
        return boost::shared_ptr< DEC_Knowledge_Agent >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::HasKnowledgeAgent
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_BlackBoard_CanContainKnowledgeAgent::HasKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const
{
    return realAgentMap_.find( &agentKnown ) != realAgentMap_.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgentFromID( unsigned int nID ) const
{
    CIT_KnowledgeAgentIDMap itKnowledge = unitKnowledgeFromIDMap_.find( nID );
    return itKnowledge == unitKnowledgeFromIDMap_.end() ? boost::shared_ptr< DEC_Knowledge_Agent >() : itKnowledge->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::SaveAllCurrentKnowledgeAgent
// Created: SLG 2010-06-07
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::SaveAllCurrentKnowledgeAgent()
{
    previousAgentMap_ = realAgentMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetDetected
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetDetected()
{
    UpdateQueriesCache();
    return detectedContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetFriends
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetFriends()
{
    UpdateQueriesCache();
    return friendsContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetEnemies
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetEnemies()
{
    UpdateQueriesCache();
    return enemiesContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetMilitias
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetMilitias()
{
    UpdateQueriesCache();
    return militiasContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetTerrorists
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetTerrorists()
{
    UpdateQueriesCache();
    return terroristsContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetRefugees
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetRefugees()
{
    UpdateQueriesCache();
    return refugeesContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetSurrenderedAgents
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
const T_KnowledgeAgentVector& DEC_BlackBoard_CanContainKnowledgeAgent::GetSurrenderedAgents()
{
    UpdateQueriesCache();
    return surrenderedAgentsContainer_;
}

