// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectPerception.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectPerception.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_ObjectPerception.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "Network/NET_AS_MOSServerMsgMgr.h"
#include "Network/NET_AgentServer.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectPerception, "DEC_Knowledge_ObjectPerception" )

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::DEC_Knowledge_ObjectPerception( const MIL_AgentPion& agentPerceiving, MIL_RealObject_ABC& objectPerceived )
    : DEC_Knowledge_ABC        ()
    , pAgentPerceiving_        ( &agentPerceiving )
    , pObjectPerceived_        ( &objectPerceived )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::DEC_Knowledge_ObjectPerception()
    : DEC_Knowledge_ABC        ()
    , pAgentPerceiving_        ( 0 )
    , pObjectPerceived_        ( 0 )
    , pCurrentPerceptionLevel_ ( 0 )
    , pPreviousPerceptionLevel_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::~DEC_Knowledge_ObjectPerception()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );

    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
         >> pObjectPerceived_;
    
    uint nTmp;
    file >> nTmp;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::FindPerceptionLevel( nTmp );
    
    file >> nTmp;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmp );
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pAgentPerceiving_ 
         << pObjectPerceived_
         << pCurrentPerceptionLevel_->GetID()
         << pPreviousPerceptionLevel_->GetID();
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::Update
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::Update( const PHY_PerceptionLevel& perceptionLevel )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        pCurrentPerceptionLevel_ = &perceptionLevel;   
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::UpdateOnNetwork() const
{
    if ( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::SendStateToNewClient() const
{
    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN_BufferedMessage msg = msgMgr.BuildMessage();
    assert( pAgentPerceiving_ );
    msg << (uint32)pAgentPerceiving_->GetID();
    assert( pObjectPerceived_ );
    msg << (uint32)pObjectPerceived_->GetID();
    msg << (uint8)pCurrentPerceptionLevel_->GetID();
    msgMgr.SendMsgObjectInterVisibility( msg );
}
