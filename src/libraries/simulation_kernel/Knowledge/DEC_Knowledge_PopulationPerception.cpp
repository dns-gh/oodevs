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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_Population.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include <boost/serialization/split_free.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationPerception )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception( const MIL_Agent_ABC& agentPerceiving, MIL_Population& populationPerceived )
    : DEC_Knowledge_ABC()
    , pAgentPerceiving_    ( &agentPerceiving )
    , pPopulationPerceived_( &populationPerceived )
    , bAttacker_( false )

{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception()
    : DEC_Knowledge_ABC()
    , pAgentPerceiving_    ( 0 )
    , pPopulationPerceived_( 0 )
    , bAttacker_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::~DEC_Knowledge_PopulationPerception()
{
    // NOTHING
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( DEC_Knowledge_PopulationPerception::CIT_ConcentrationMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationPerception::T_ConcentrationMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while( nNbr-- )
            {
                MIL_PopulationConcentration* pConcentration;
                file >> pConcentration;
                file >> map[ pConcentration ];
            }
        }

        template< typename Archive >
        inline
        void serialize( Archive& file, DEC_Knowledge_PopulationPerception::T_FlowMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const DEC_Knowledge_PopulationPerception::T_FlowMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for( DEC_Knowledge_PopulationPerception::CIT_FlowMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first
                     << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, DEC_Knowledge_PopulationPerception::T_FlowMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while( nNbr-- )
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
void DEC_Knowledge_PopulationPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> const_cast< MIL_Agent_ABC*& >( pAgentPerceiving_ )
         >> pPopulationPerceived_
         >> concentrations_
         >> flows_
         >> bAttacker_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pAgentPerceiving_
         << pPopulationPerceived_
         << concentrations_
         << flows_
         << bAttacker_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::Prepare()
{
    bAttacker_ = false;
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
// Name: DEC_Knowledge_PopulationPerception::IsPerceived
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsPerceived() const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( it->second->IsPerceived() )
            return true;
    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        if( it->second->IsPerceived() )
            return true;
    return false;
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
void DEC_Knowledge_PopulationPerception::SendStateToNewClient( unsigned int nCtx ) const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient( nCtx );
    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->SendStateToNewClient( nCtx );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::GetPopulationPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
MIL_Population& DEC_Knowledge_PopulationPerception::GetPopulationPerceived() const
{
    assert( pPopulationPerceived_ );
    return *pPopulationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::GetPopulationPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_PopulationPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::NotifyAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::NotifyAttacker()
{
    bAttacker_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::IsAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsAttacker() const
{
    return bAttacker_;
}
