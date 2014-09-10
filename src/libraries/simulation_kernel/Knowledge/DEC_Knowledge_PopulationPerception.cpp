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

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationPerception )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception( const MIL_Agent_ABC& agentPerceiving, MIL_Population& populationPerceived )
    : pAgentPerceiving_    ( &agentPerceiving )
    , pPopulationPerceived_( &populationPerceived )
    , bAttacker_           ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationPerception::DEC_Knowledge_PopulationPerception()
    : pAgentPerceiving_    ( 0 )
    , pPopulationPerceived_( 0 )
    , bAttacker_           ( false )
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> pAgentPerceiving_;
    file >> pPopulationPerceived_;
    file >> concentrations_;
    file >> flows_;
    file >> bAttacker_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << pAgentPerceiving_;
    file << pPopulationPerceived_;
    file << concentrations_;
    file << flows_;
    file << bAttacker_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::Prepare()
{
    bAttacker_ = false;
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->Prepare();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
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
    for( auto it = concentrations_.begin(); it != concentrations_.end(); )
    {
        DEC_Knowledge_PopulationConcentrationPerception* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = concentrations_.erase( it );
        }
        else
            ++it;
    }
    for( auto it = flows_.begin(); it != flows_.end(); )
    {
        DEC_Knowledge_PopulationFlowPerception* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = flows_.erase( it );
        }
        else
            ++it;
    }
    return concentrations_.empty() && flows_.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsIdentified( const MIL_PopulationConcentration& concentration )
{
    auto it = concentrations_.find( &concentration );
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
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        if( it->second->IsPerceived() )
            return true;
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
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
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        knowledge.Update( *it->second );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        knowledge.Update( *it->second );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::UpdateOnNetwork() const
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateOnNetwork();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationPerception::SendStateToNewClient( unsigned int nCtx ) const
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient( nCtx );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
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
// Name: DEC_Knowledge_PopulationPerception::IsDestructingUrbanblocks
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsDestructingUrbanblocks() const
{
    assert( pPopulationPerceived_ );
    return pPopulationPerceived_->GetUrbanDestructionState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationPerception::IsDemonstrating
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationPerception::IsDemonstrating() const
{
    assert( pPopulationPerceived_ );
    return pPopulationPerceived_->GetDemonstrationState();
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
