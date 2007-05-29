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

#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"

using namespace DIN;

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentPerception, "DEC_Knowledge_AgentPerception" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::DEC_Knowledge_AgentPerception( const MIL_AgentPion& agentPerceiving, MIL_Agent_ABC& agentPerceived )
    : DEC_Knowledge_ABC          ()
    , pAgentPerceiving_          ( &agentPerceiving )
    , pAgentPerceived_           ( &agentPerceived )
    , pCurrentPerceptionLevel_   ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , dataDetection_             ()
    , dataRecognition_           ()
    , dataIdentification_        ()
    , bRecordModeEnabled_        ( false )
    , bPreviousRecordModeEnabled_( false )
    , nCreationTimeStep_         ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , nRecordModeDisablingDelay_ ( 0 )
    , bAttacker_                 ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::DEC_Knowledge_AgentPerception()
    : DEC_Knowledge_ABC          ()
    , nCreationTimeStep_         ( 0 )
    , dataDetection_             ()
    , dataRecognition_           ()
    , dataIdentification_        ()
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
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerception::~DEC_Knowledge_AgentPerception()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Serialize
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID(),
             max      = pMaxPerceptionLevel_->GetID();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << nCreationTimeStep_
         << dataDetection_
         << dataRecognition_
         << dataIdentification_
         << pAgentPerceiving_
         << pAgentPerceived_
         << bRecordModeEnabled_
         << bPreviousRecordModeEnabled_
         << nRecordModeDisablingDelay_
         << bAttacker_
         << current
         << previous
         << max;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::Unserialize
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> nCreationTimeStep_
         >> dataDetection_
         >> dataRecognition_
         >> dataIdentification_
         >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ )
         >> pAgentPerceived_
         >> bRecordModeEnabled_
         >> bPreviousRecordModeEnabled_
         >> nRecordModeDisablingDelay_
         >> bAttacker_;
    
    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    
    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    
    file >> nID;
    pMaxPerceptionLevel_      = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
    if( *pCurrentPerceptionLevel_ < PHY_PerceptionLevel::recognized_ && pAgentPerceived_->IsDead() )
        pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::recognized_;

    if( *pCurrentPerceptionLevel_ > *pMaxPerceptionLevel_ )
        pMaxPerceptionLevel_ = pCurrentPerceptionLevel_;

    assert( pAgentPerceived_ );
    dataDetection_     .Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
    dataRecognition_   .Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
    dataIdentification_.Update( *pAgentPerceived_, *pCurrentPerceptionLevel_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::SendStateToNewClient() const
{
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();
    assert( pAgentPerceiving_ );
    assert( pAgentPerceived_ );
    msg << (uint32)pAgentPerceiving_->GetID();
    msg << (uint32)pAgentPerceived_ ->GetID();
    msg << (uint8)pCurrentPerceptionLevel_->GetID();
    msg << (uint8)pMaxPerceptionLevel_    ->GetID();
    msg << bRecordModeEnabled_;
    msgMgr.SendMsgUnitInterVisibility( msg );        
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerception::UpdateOnNetwork() const
{
    if ( *pCurrentPerceptionLevel_ == *pPreviousPerceptionLevel_ && bPreviousRecordModeEnabled_ == bRecordModeEnabled_ )
        return;

    SendStateToNewClient();
}
