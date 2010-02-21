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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectPerception )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectPerception::DEC_Knowledge_ObjectPerception( const MIL_AgentPion& agentPerceiving, MIL_Object_ABC& objectPerceived )
    : DEC_Knowledge_ABC        ()
    , pAgentPerceiving_        ( &agentPerceiving )
    , pObjectPerceived_        ( &objectPerceived )
    , pCurrentPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_( &PHY_PerceptionLevel::notSeen_ )
{
    // NOTHING
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

    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
         >> pObjectPerceived_;
    
    unsigned int nTmp;
    file >> nTmp;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::FindPerceptionLevel( nTmp );
    
    file >> nTmp;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmp );
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectPerception::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pAgentPerceiving_ 
         << pObjectPerceived_
         << current
         << previous;
}

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
    client::ObjectDetection asn;
    asn().set_oid( pAgentPerceiving_->GetID() );
    asn().set_object_oid( pObjectPerceived_->GetID() );
    asn().set_visibility( Common::EnumUnitVisibility( pCurrentPerceptionLevel_->GetID() ) );
    asn.Send( NET_Publisher_ABC::Publisher() );
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
const MIL_AgentPion& DEC_Knowledge_ObjectPerception::GetAgentPerceiving() const
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
