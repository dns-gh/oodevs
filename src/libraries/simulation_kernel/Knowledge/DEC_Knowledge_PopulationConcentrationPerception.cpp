// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentrationPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationConcentrationPerception.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_PopulationConcentrationPerception )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentrationPerception::DEC_Knowledge_PopulationConcentrationPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationConcentration& concentrationPerceived )
    : pPopulationKnowledge_             ( &populationKnowledge )
    , pPopulationConcentrationPerceived_( &concentrationPerceived )
    , pCurrentPerceptionLevel_          ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_         ( &PHY_PerceptionLevel::notSeen_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentrationPerception::DEC_Knowledge_PopulationConcentrationPerception()
    : pPopulationKnowledge_             ( 0 )
    , pPopulationConcentrationPerceived_( 0 )
    , pCurrentPerceptionLevel_          ( 0 )
    , pPreviousPerceptionLevel_         ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentrationPerception::~DEC_Knowledge_PopulationConcentrationPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ );
    file >> pPopulationConcentrationPerceived_;
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pPopulationKnowledge_;
    file << pPopulationConcentrationPerceived_;
    file << pCurrentPerceptionLevel_;
    file << pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::Update( const PHY_PerceptionLevel& perceptionLevel )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentrationPerception::Clean()
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ == PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentrationPerception::IsIdentified()
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ == PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::IsPerceived
// Created: SBO 2006-01-18
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentrationPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::IsDestructingUrbanblocks
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentrationPerception::IsDestructingUrbanblocks() const
{
    assert( pPopulationKnowledge_ );
    return pPopulationKnowledge_->IsDestructingUrbanblocks();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::IsDemonstrating
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentrationPerception::IsDemonstrating() const
{
    assert( pPopulationKnowledge_ );
    return pPopulationKnowledge_->IsDemonstrating();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetNbrAliveHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_PopulationConcentrationPerception::GetNbrAliveHumans() const
{
    assert( pPopulationConcentrationPerceived_ );
    return pPopulationConcentrationPerceived_->GetTotalLivingHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetNbrDeadHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_PopulationConcentrationPerception::GetNbrDeadHumans() const
{
    assert( pPopulationConcentrationPerceived_ );
    return pPopulationConcentrationPerceived_->GetDeadHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetAttitude
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& DEC_Knowledge_PopulationConcentrationPerception::GetAttitude() const
{
    assert( pPopulationConcentrationPerceived_ );
    return pPopulationConcentrationPerceived_->GetAttitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetAgentPerceiving
// Created: JSR 2013-05-28
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_PopulationConcentrationPerception::GetAgentPerceiving() const
{
    return pPopulationKnowledge_->GetAgentPerceiving();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::UpdateOnNetwork() const
{
    if( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::SendStateToNewClient( unsigned int nCtx ) const
{
    client::CrowdConcentrationDetection asn;
    asn().mutable_observer()->set_id( pPopulationKnowledge_->GetAgentPerceiving().GetID() );
    asn().mutable_detected_crowd()->set_id( pPopulationKnowledge_->GetPopulationPerceived().GetID() );
    asn().mutable_detected_concentration()->set_id( pPopulationConcentrationPerceived_->GetID() );
    asn().set_visibility( sword::UnitVisibility::Level( pCurrentPerceptionLevel_->GetID() ) );
    asn.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetConcentrationPerceived
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
const MIL_PopulationConcentration& DEC_Knowledge_PopulationConcentrationPerception::GetConcentrationPerceived() const
{
    assert( pPopulationConcentrationPerceived_ );
    return *pPopulationConcentrationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetCurrentPerceptionLevel
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_PopulationConcentrationPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}
