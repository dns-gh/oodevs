// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationPerception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationPerception.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_Population.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationPerception, "DEC_Knowledge_PopulationPerception" )

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception( const MIL_AgentPion& agentPerceiving, MIL_Population& populationPerceived )
    : DEC_Knowledge_ABC    ()
    , pAgentPerceiving_    ( &agentPerceiving )
    , pPopulationPerceived_( &populationPerceived )
    , concentrations_      ()
    , flows_               ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception()
    : DEC_Knowledge_ABC    ()
    , pAgentPerceiving_    ( 0 )
    , pPopulationPerceived_( 0 )
    , concentrations_      ()
    , flows_               ()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::~DEC_Knowledge_PopulationPerception()
{
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
        void serialize( Archive& file, DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const uint )
        {
            file << map.size();
            for ( DEC_Knowledge_PopulationPerception::CIT_ConcentrationMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_PopulationConcentration* pConcentration;
                file >> pConcentration;
                file >> map[ pConcentration ];
            }
        }

        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_Knowledge_PopulationPerception::T_FlowMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationPerception::T_FlowMap& map, const uint )
        {
            file << map.size();
            for ( DEC_Knowledge_PopulationPerception::CIT_FlowMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }
        
        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationPerception::T_FlowMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                MIL_PopulationFlow* pFlow;
                file >> pFlow;
                file >> map[ pFlow ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );

    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ )
         >> pPopulationPerceived_
         >> concentrations_
         >> flows_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << const_cast< MIL_AgentPion*& >( pAgentPerceiving_ )
         << pPopulationPerceived_
         << concentrations_
         << flows_;
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::Prepare()
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->Prepare();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->Prepare();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::Update( MIL_PopulationConcentration& concentrationPerceived, const PHY_PerceptionLevel& perceptionLevel )
{
    DEC_Knowledge_PopulationConcentrationPerception*& pKnowledge = concentrations_[ &concentrationPerceived ];
    if( !pKnowledge )
        pKnowledge = new DEC_Knowledge_PopulationConcentrationPerception( *this, concentrationPerceived );
    pKnowledge->Update( perceptionLevel );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::Update( MIL_PopulationFlow& flowPerceived, const PHY_PerceptionLevel& perceptionLevel, const T_PointVector& shape )
{
    DEC_Knowledge_PopulationFlowPerception*& pKnowledge = flows_[ &flowPerceived ];
    if( !pKnowledge )
        pKnowledge = new DEC_Knowledge_PopulationFlowPerception( *this, flowPerceived );
    pKnowledge->Update( perceptionLevel, shape );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::Clean()
{
    for( IT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); )
    {
        DEC_Knowledge_PopulationConcentrationPerception* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = concentrations_.erase( it );
        }
        else 
            ++ it;
    }

    for( IT_FlowMap it = flows_.begin(); it != flows_.end(); )
    {
        DEC_Knowledge_PopulationFlowPerception* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = flows_.erase( it );
        }
        else 
            ++ it;
    }

    return concentrations_.empty() && flows_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsIdentified( const MIL_PopulationConcentration& concentration )
{
    CIT_ConcentrationMap it = concentrations_.find( &concentration );
    if( it == concentrations_.end() )
        return false;
    return it->second->IsIdentified();
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::PublishKnowledges
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::PublishKnowledges( DEC_Knowledge_Population& knowledge ) const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        knowledge.Update( *it->second );

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        knowledge.Update( *it->second );
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::UpdateOnNetwork() const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateOnNetwork();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::SendStateToNewClient() const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->SendStateToNewClient();
}
