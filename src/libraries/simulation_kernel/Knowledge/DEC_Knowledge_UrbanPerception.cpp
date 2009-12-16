// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_UrbanPerception.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_IDManager.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "urban/TerrainObject_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_UrbanPerception, "DEC_Knowledge_UrbanPerception" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception constructor
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception::DEC_Knowledge_UrbanPerception( const MIL_AgentPion& agentPerceiving, const urban::TerrainObject_ABC& object )
    : DEC_Knowledge_ABC              ()
    , perceiver_                     ( agentPerceiving )
    , object_                        ( object )
    , nID_                           ( MIL_IDManager::GetFreeId() )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception destructor
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
DEC_Knowledge_UrbanPerception::~DEC_Knowledge_UrbanPerception()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::load
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> const_cast< uint& >( nID_ );
    
    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    
    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::save
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << nID_
         << current
         << previous;
}

// =============================================================================
// INTERNAL UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::Prepare
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::Prepare()
{
    //@TODO MGD
}
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::Update
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::Update( const PHY_PerceptionLevel& perceptionLevel )
{
    assert( perceptionLevel != PHY_PerceptionLevel::notSeen_ );
    if( perceptionLevel > *pCurrentPerceptionLevel_ )
        SetPerceptionLevel( perceptionLevel );   
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::SetPerceptionLevel
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::SetPerceptionLevel( const PHY_PerceptionLevel& perceptionLevel )
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_ = &perceptionLevel;
}

// =============================================================================
// NETWORK UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::UpdateOnNetwork
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::UpdateOnNetwork()
{
    if ( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ )
        SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::SendStateToNewClient
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_UrbanPerception::SendStateToNewClient()
{
    NET_ASN_MsgUrbanDetection asn;
    asn().oid        = perceiver_.GetID();
    asn().urban_oid  = object_.GetId();
    asn().visibility = ASN1T_EnumUnitVisibility( pCurrentPerceptionLevel_->GetID() );
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::Clean
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
bool DEC_Knowledge_UrbanPerception::Clean() const
{
    return !IsPerceived();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::IsValid
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
unsigned DEC_Knowledge_UrbanPerception::GetId  () const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::GetLevel
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_UrbanPerception::GetCurrentPerceptionLevel() const
{
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::GetUrbanPerceived
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC& DEC_Knowledge_UrbanPerception::GetUrbanPerceived() const
{
    return object_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_UrbanPerception::GetPerceiver
// Created: MGD 2009-12-14
// -----------------------------------------------------------------------------
const MIL_AgentPion& DEC_Knowledge_UrbanPerception::GetPerceiver() const
{
    return perceiver_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectPerception::IsPerceived
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
bool DEC_Knowledge_UrbanPerception::IsPerceived() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_;
}
