// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationFlowPerception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"

#include "DEC_Knowledge_PopulationPerception.h"
#include "MIL_AgentServer.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationFlowPerception, "DEC_Knowledge_PopulationFlowPerception" )

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::DEC_Knowledge_PopulationFlowPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationFlow& flowPerceived )
    : pPopulationKnowledge_    ( &populationKnowledge    )
    , pPopulationFlowPerceived_( &flowPerceived )
    , shape_                   ()
    , previousShape_           ()
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::DEC_Knowledge_PopulationFlowPerception()
    : pPopulationKnowledge_    ( 0 )
    , pPopulationFlowPerceived_( 0 )
    , shape_                   ()
    , previousShape_           ()
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlowPerception::~DEC_Knowledge_PopulationFlowPerception()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ )
         >> pPopulationFlowPerceived_
         >> shape_
         >> previousShape_;

    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );

    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ )
         << pPopulationFlowPerceived_
         << shape_
         << previousShape_
         << pCurrentPerceptionLevel_->GetID()
         << pPreviousPerceptionLevel_->GetID();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;

    previousShape_.clear();
    shape_.swap( previousShape_ );    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::Update( const PHY_PerceptionLevel& perceptionLevel, const T_PointVector& shape )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;   

    shape_                   = shape;
    pCurrentPerceptionLevel_ = &perceptionLevel;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationFlowPerception::Clean()
{
    return shape_.empty();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetNbrAliveHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
uint DEC_Knowledge_PopulationFlowPerception::GetNbrAliveHumans() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetNbrAliveHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetNbrDeadHumans
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
uint DEC_Knowledge_PopulationFlowPerception::GetNbrDeadHumans() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetNbrDeadHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetAttitude
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& DEC_Knowledge_PopulationFlowPerception::GetAttitude() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetAttitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetShape
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const T_PointVector& DEC_Knowledge_PopulationFlowPerception::GetShape() const
{
    return shape_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetDirection
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_PopulationFlowPerception::GetDirection() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::GetSpeed
// Created: NLD 2005-10-14
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationFlowPerception::GetSpeed() const
{
    assert( pPopulationFlowPerceived_ );
    return pPopulationFlowPerceived_->GetSpeed();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::UpdateOnNetwork() const
{
    if( shape_ != previousShape_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationFlowPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationFlowPerception::SendStateToNewClient() const
{
    assert( pPopulationKnowledge_ );
    assert( pPopulationFlowPerceived_ );

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();
    msg << (uint32)pPopulationKnowledge_->GetAgentPerceiving().GetID();
    msg << (uint32)pPopulationKnowledge_->GetPopulationPerceived().GetID();
    msg << (uint32)pPopulationFlowPerceived_->GetID();

    msg << (uint32)shape_.size();
    for( CIT_PointVector it = shape_.begin(); it != shape_.end(); ++it )
        msg << *it;
    msgMgr.SendMsgPopulationFlowInterVisibility( msg );
}
