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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectPerception )

namespace {
    int scounterTmp = 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::DEC_Knowledge_ObjectPerception( const MIL_Agent_ABC& agentPerceiving, MIL_Object_ABC& objectPerceived )
    : DEC_Knowledge_ABC()
    , pAgentPerceiving_        ( &agentPerceiving )
    , pObjectPerceived_        ( &objectPerceived )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
    , counterTmp_( scounterTmp++ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::DEC_Knowledge_ObjectPerception()
    : DEC_Knowledge_ABC()
    , pAgentPerceiving_        ( 0 )
    , pObjectPerceived_        ( 0 )
    , pCurrentPerceptionLevel_ ( 0 )
    , pPreviousPerceptionLevel_( 0 )
    , counterTmp_( scounterTmp++ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::~DEC_Knowledge_ObjectPerception()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> const_cast< MIL_Agent_ABC*& >( pAgentPerceiving_ );
    file >> pObjectPerceived_;
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << pAgentPerceiving_;
    file << pObjectPerceived_;
    file << pCurrentPerceptionLevel_;
    file << pPreviousPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
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
    if( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::SendStateToNewClient( unsigned int nCtx ) const
{
    client::ObjectDetection asn;
    asn().mutable_observer()->set_id( pAgentPerceiving_->GetID() );
    asn().mutable_detected_object()->set_id( pObjectPerceived_->GetID() );
    asn().set_visibility( sword::UnitVisibility::Level( pCurrentPerceptionLevel_->GetID() ) );
    asn.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetObjectPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
MIL_Object_ABC& DEC_Knowledge_ObjectPerception::GetObjectPerceived() const
{
    assert( pObjectPerceived_ );
    return *pObjectPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetObjectPerceiving
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
const MIL_Agent_ABC& DEC_Knowledge_ObjectPerception::GetAgentPerceiving() const
{
    assert( pAgentPerceiving_ );
    return *pAgentPerceiving_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::IsPerceived
// Created: NLD 2004-03-23
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::GetCurrentPerceptionLevel
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_ObjectPerception::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_ObjectPerception::Clean()
{
    return !IsPerceived();
}
