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

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"

#include "DEC_Knowledge_PopulationPerception.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationConcentrationPerception, "DEC_Knowledge_PopulationConcentrationPerception" )

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentrationPerception::DEC_Knowledge_PopulationConcentrationPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationConcentration& concentrationPerceived )
    : pPopulationKnowledge_             ( &populationKnowledge    )
    , pPopulationConcentrationPerceived_( &concentrationPerceived )
    , pCurrentPerceptionLevel_          ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_         ( &PHY_PerceptionLevel::notSeen_ )
{
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
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentrationPerception::~DEC_Knowledge_PopulationConcentrationPerception()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ )
         >> pPopulationConcentrationPerceived_;

    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );

    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentrationPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << const_cast< DEC_Knowledge_PopulationPerception*& >( pPopulationKnowledge_ )
         << pPopulationConcentrationPerceived_
         << pCurrentPerceptionLevel_->GetID()
         << pPreviousPerceptionLevel_->GetID();
}

// =============================================================================
// OPERATIONS
// =============================================================================

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

// =============================================================================
// ACCESSORS
// =============================================================================
        
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetNbrAliveHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationConcentrationPerception::GetNbrAliveHumans() const
{
    assert( pPopulationConcentrationPerceived_ );
    return pPopulationConcentrationPerceived_->GetNbrAliveHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentrationPerception::GetNbrDeadHumans
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_PopulationConcentrationPerception::GetNbrDeadHumans() const
{
    assert( pPopulationConcentrationPerceived_ );
    return pPopulationConcentrationPerceived_->GetNbrDeadHumans();
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

// =============================================================================
// NETWORK
// =============================================================================

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
void DEC_Knowledge_PopulationConcentrationPerception::SendStateToNewClient() const
{
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();
    assert( pPopulationKnowledge_ );
    assert( pPopulationConcentrationPerceived_ );
    assert( pCurrentPerceptionLevel_ );
    msg << (uint32)pPopulationKnowledge_->GetAgentPerceiving().GetID();
    msg << (uint32)pPopulationKnowledge_->GetPopulationPerceived().GetID();
    msg << (uint32)pPopulationConcentrationPerceived_->GetID();
    msg << (uint8)pCurrentPerceptionLevel_->GetID();
    msgMgr.SendMsgPopulationConcentrationInterVisibility( msg );
}
