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
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"

BOOST_CLASS_EXPORT_GUID( DEC_BlackBoard_CanContainKnowledgeAgent, "DEC_BlackBoard_CanContainKnowledgeAgent" )

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
        void serialize( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint )
        {
            const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap::size_type size = map.size();
            file << size;
            for ( DEC_BlackBoard_CanContainKnowledgeAgent::CIT_KnowledgeAgentMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_Agent_ABC*       pAgent;
                DEC_Knowledge_Agent* pKnowledge;
                
                file >> pAgent
                     >> pKnowledge;
                     
                map.insert( std::make_pair( pAgent, pKnowledge ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ )
         >> nLastCacheUpdateTick_
         >> realAgentMap_
         >> unitKnowledgeFromIDMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << pKnowledgeGroup_
         << nLastCacheUpdateTick_
         << realAgentMap_
         << unitKnowledgeFromIDMap_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown )
{
    DEC_Knowledge_Agent& knowledge = agentKnown.CreateKnowledge( knowledgeGroup );
    bool bOut = realAgentMap_.insert( std::make_pair( &agentKnown, &knowledge ) ).second;
    assert( bOut );

    bOut = unitKnowledgeFromIDMap_.insert( std::make_pair( knowledge.GetID(), &knowledge ) ).second;
    assert( bOut );

    return knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    int nOut = realAgentMap_.erase( &knowledge.GetAgentKnown() );
    assert( nOut >= 1 );
    nOut = unitKnowledgeFromIDMap_.erase( knowledge.GetID() );
    assert( nOut == 1 );
    delete &knowledge;
}

// =============================================================================
// QUERIES
// =============================================================================

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
    const MIL_Army& army = pKnowledgeGroup_->GetArmy();

    for( CIT_KnowledgeAgentMap itKnowledge = realAgentMap_.begin(); itKnowledge != realAgentMap_.end(); ++itKnowledge )
    {
        DEC_Knowledge_Agent& knowledge = *itKnowledge->second;

        if( knowledge.IsRefugee() )
            refugeesContainer_.push_back( &knowledge );

        else if( knowledge.IsSurrendered() )
        {
            if( !knowledge.GetArmy() || *knowledge.GetArmy() != army ) 
                surrenderedAgentsContainer_.push_back( &knowledge );
        }

        else
        {
            if( !knowledge.GetArmy() )
            {
                if( army.IsAFriend( knowledge.GetAgentKnown().GetArmy() ) != eTristate_True )
                    detectedContainer_.push_back( &knowledge );
            }

            else if( army.IsAFriend( knowledge ) == eTristate_True )
                friendsContainer_.push_back( &knowledge );
            
            else if( army.IsAnEnemy( knowledge ) == eTristate_True )
                enemiesContainer_.push_back( &knowledge );

            if( knowledge.IsMilitia() )
                militiasContainer_.push_back( &knowledge );

            if( knowledge.IsTerrorist() )
                terroristsContainer_.push_back( &knowledge );
        }
    }
}

