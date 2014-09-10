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
#include "MIL_Time_ABC.h"
#include "KnowledgesVisitor_ABC.h"
#include "MIL_KnowledgeGroup.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_Army.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_BlackBoard_CanContainKnowledgeAgent )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent constructor
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
DEC_BlackBoard_CanContainKnowledgeAgent::DEC_BlackBoard_CanContainKnowledgeAgent( MIL_KnowledgeGroup* knowledgeGroup )
    : pKnowledgeGroup_           ( knowledgeGroup )
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

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::load
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ );
    file >> nLastCacheUpdateTick_;
    file >> realAgentMap_;
    file >> previousAgentMap_;
    for( auto it = realAgentMap_.begin(); it != realAgentMap_.end(); ++it )
        unitKnowledgeFromIDMap_[ it->second->GetID() ] = it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::save
// Created: JVT 2005-04-06
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pKnowledgeGroup_;
    file << nLastCacheUpdateTick_;
    file << realAgentMap_;
    file << previousAgentMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent& DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent( const MIL_KnowledgeGroup& group, const MIL_Agent_ABC& agentKnown )
{
    boost::shared_ptr< DEC_Knowledge_Agent > knowledge = const_cast< MIL_Agent_ABC& >( agentKnown ).CreateKnowledge( group );
    if( ! realAgentMap_.insert( std::make_pair( &agentKnown, knowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    if( ! unitKnowledgeFromIDMap_.insert( std::make_pair( knowledge->GetID(), knowledge ) ).second )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Insert failed" );
    return *knowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    knowledge.Invalidate();
    previousAgentMap_.erase( &knowledge.GetAgentKnown() );
    if( realAgentMap_.erase( &knowledge.GetAgentKnown() ) < 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
    if( unitKnowledgeFromIDMap_.erase( knowledge.GetID() ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::UpdateQueriesCache
// Created: NLD 2006-04-13
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::UpdateQueriesCache()
{
    if( nLastCacheUpdateTick_ >= MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
        return;

    nLastCacheUpdateTick_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();

    detectedContainer_         .clear();
    friendsContainer_          .clear();
    enemiesContainer_          .clear();
    militiasContainer_         .clear();
    terroristsContainer_       .clear();
    refugeesContainer_         .clear();
    surrenderedAgentsContainer_.clear();

    assert( pKnowledgeGroup_ );
    const MIL_Army_ABC& army = pKnowledgeGroup_->GetArmy();

    for( auto it = realAgentMap_.begin(); it != realAgentMap_.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledge = it->second;

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
    auto it = realAgentMap_.find( &agentKnown );
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
    auto it = unitKnowledgeFromIDMap_.find( nID );
    return it == unitKnowledgeFromIDMap_.end() ? boost::shared_ptr< DEC_Knowledge_Agent >() : it->second;
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

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    visitor.VisitKnowledgesAgent( realAgentMap_.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgents
// Created: MCO 2012-05-30
// -----------------------------------------------------------------------------
const DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap& DEC_BlackBoard_CanContainKnowledgeAgent::GetKnowledgeAgents() const
{
    return realAgentMap_;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent
// Created: LGY 2013-03-27
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::CreateKnowledgeAgent( const DEC_Knowledge_Agent& knowledge )
{
    boost::shared_ptr< DEC_Knowledge_Agent > copy( new DEC_Knowledge_Agent( knowledge, *pKnowledgeGroup_ ) );
    realAgentMap_.insert( std::make_pair( &knowledge.GetAgentKnown(), copy ) );
    unitKnowledgeFromIDMap_.insert( std::make_pair( copy->GetID(), copy ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeAgent::Merge
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void DEC_BlackBoard_CanContainKnowledgeAgent::Merge( const DEC_BlackBoard_CanContainKnowledgeAgent& subGroup )
{
    for( auto itKnowledge = subGroup.realAgentMap_.begin(); itKnowledge != subGroup.realAgentMap_.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = GetKnowledgeAgent( *itKnowledge->first );
        boost::shared_ptr< DEC_Knowledge_Agent > pSubKnowledge = itKnowledge->second;
        if( !pKnowledge.get() )
            CreateKnowledgeAgent( *pSubKnowledge );
        else if( pKnowledge->GetRelevance() < pSubKnowledge->GetRelevance() )
        {
            DestroyKnowledgeAgent( *pKnowledge );
            CreateKnowledgeAgent( *pSubKnowledge );
        }
    }
}
