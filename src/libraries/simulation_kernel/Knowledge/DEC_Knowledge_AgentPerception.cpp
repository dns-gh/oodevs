// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 13:00 $
// $Revision: 8 $
// $Workfile: DEC_Knowledge_AgentPerception.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "MIL_Time_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentPerception )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::DEC_Knowledge_AgentPerception( const MIL_Agent_ABC& agentPerceiving, const MIL_Agent_ABC& agentPerceived )
    : nCreationTimeStep_         ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
    , pAgentPerceiving_          ( &agentPerceiving )
    , pAgentPerceived_           ( &agentPerceived )
    , pCurrentPerceptionLevel_   ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , nRecordModeDisablingDelay_ ( 0 )
    , bRecordModeEnabled_        ( false )
    , bPreviousRecordModeEnabled_( false )
    , bAttacker_                 ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::DEC_Knowledge_AgentPerception()
    : nCreationTimeStep_         ( 0 )
    , pAgentPerceiving_          ( 0 )
    , pAgentPerceived_           ( 0 )
    , pCurrentPerceptionLevel_   ( 0 )
    , pPreviousPerceptionLevel_  ( 0 )
    , pMaxPerceptionLevel_       ( 0 )
    , bRecordModeEnabled_        ( false )
    , bPreviousRecordModeEnabled_( false )
    , nRecordModeDisablingDelay_ ( 0 )
    , bAttacker_                 ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::~DEC_Knowledge_AgentPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Serialize
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << nCreationTimeStep_;
    file << dataDetection_;
    file << dataRecognition_;
    file << dataIdentification_;
    file << pAgentPerceiving_;
    file << pAgentPerceived_;
    file << bRecordModeEnabled_;
    file << bPreviousRecordModeEnabled_;
    file << nRecordModeDisablingDelay_;
    file << bAttacker_;
    file << pCurrentPerceptionLevel_;
    file << pPreviousPerceptionLevel_;
    file << pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Unserialize
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> nCreationTimeStep_;
    file >> dataDetection_;
    file >> dataRecognition_;
    file >> dataIdentification_;
    file >> const_cast< MIL_Agent_ABC*& >( pAgentPerceiving_ );
    file >> pAgentPerceived_;
    file >> bRecordModeEnabled_;
    file >> bPreviousRecordModeEnabled_;
    file >> nRecordModeDisablingDelay_;
    file >> bAttacker_;
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
    file >> pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;
        assert( pAgentPerceived_ );
        dataDetection_.Prepare( *pAgentPerceived_ );
    }
    bAttacker_ = false;
    bPreviousRecordModeEnabled_ = bRecordModeEnabled_;
    if( nRecordModeDisablingDelay_ > 0 && --nRecordModeDisablingDelay_ == 0 )
        bRecordModeEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::Update( const PHY_PerceptionLevel& perceptionLevel, bool bRecordModeEnabled )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    assert( pCurrentPerceptionLevel_ );
    bRecordModeEnabled_ = bRecordModeEnabled;
    // On garde le niveau le plus haut
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;
    // $$$ bDummy demandé par DSRO : quand un pion voit une unité détruite, il la reconnait afin de ne pas en avoir peur ...
    if( DEC_Knowledge_Agent::detectDestroyedUnits_ && *pCurrentPerceptionLevel_ < PHY_PerceptionLevel::recognized_ && pAgentPerceived_->IsDead() )
        pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::recognized_;
    if( *pCurrentPerceptionLevel_ > *pMaxPerceptionLevel_ )
        pMaxPerceptionLevel_ = pCurrentPerceptionLevel_;
    assert( pAgentPerceived_ );
    dataDetection_.Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
    dataRecognition_.Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
    dataIdentification_.Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::SendStateToNewClient( unsigned int nCtx ) const
{
    client::UnitDetection asn;
    asn().mutable_observer()->set_id( pAgentPerceiving_->GetID() );
    asn().mutable_detected_unit()->set_id( pAgentPerceived_->GetID() );
    asn().set_current_visibility( bRecordModeEnabled_ ? sword::UnitVisibility::recorded : sword::UnitVisibility::Level( pCurrentPerceptionLevel_->GetID() ) );
    asn().set_max_visibility( sword::UnitVisibility::Level( pMaxPerceptionLevel_->GetID() ) );
    asn.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::UpdateOnNetwork() const
{
    if( *pCurrentPerceptionLevel_ == *pPreviousPerceptionLevel_ && bPreviousRecordModeEnabled_ == bRecordModeEnabled_ )
        return;
    if( pAgentPerceived_->IsMarkedForDestruction() || pAgentPerceiving_->IsMarkedForDestruction() )
        return;
    SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetAgentPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_AgentPerception::GetAgentPerceived() const
{
    assert( pAgentPerceived_ );
    return *pAgentPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetAgentPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_AgentPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetMaxPerceptionLevel
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetMaxPerceptionLevel() const
{
    assert( pMaxPerceptionLevel_ );
    return *pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetCurrentPerceptionLevel
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetPreviousPerceptionLevel
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_AgentPerception::GetPreviousPerceptionLevel() const
{
    assert( pPreviousPerceptionLevel_ );
    return *pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsPerceived
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ || bRecordModeEnabled_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetDetectionData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentPerceptionDataDetection& DEC_Knowledge_AgentPerception::GetDetectionData() const
{
    return dataDetection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetRecognitionData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentPerceptionDataRecognition& DEC_Knowledge_AgentPerception::GetRecognitionData() const
{
    return dataRecognition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetIdentificationData
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentPerceptionDataIdentification& DEC_Knowledge_AgentPerception::GetIdentificationData() const
{
    return dataIdentification_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsAvailable
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerception::IsAvailable() const
{
    return !bRecordModeEnabled_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::MakeAvailable
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::MakeAvailable( unsigned int nDelay )
{
    if( bRecordModeEnabled_ )
    {
        nRecordModeDisablingDelay_ = nDelay;
        bRecordModeEnabled_ = ( nRecordModeDisablingDelay_ != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::GetCreationTimeStep
// Created: NLD 2004-11-16
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentPerception::GetCreationTimeStep() const
{
    return nCreationTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerception::Clean()
{
    return !IsPerceived();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::NotifyAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::NotifyAttacker()
{
    bAttacker_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::IsAttacker
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerception::IsAttacker() const
{
    return bAttacker_;
}
