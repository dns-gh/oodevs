// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Limit.h"

#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_Limit, "MIL_Limit" )

// -----------------------------------------------------------------------------
// Name: MIL_Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Limit::MIL_Limit()
    : MIL_TacticalLine_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Limit::MIL_Limit( const ASN1T_MsgLimitCreationRequest& asn )
    : MIL_TacticalLine_ABC( asn )
{
    SendMsg< NET_ASN_MsgLimitCreation >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Limit::MIL_Limit( const MIL_Automate& automateBroadcasted, MIL_InputArchive& archive )
    : MIL_TacticalLine_ABC( automateBroadcasted, archive )
{
    SendMsg< NET_ASN_MsgLimitCreation >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Limit::MIL_Limit( const MIL_Formation& formationBroadcasted, MIL_InputArchive& archive )
    : MIL_TacticalLine_ABC( formationBroadcasted, archive )
{
    SendMsg< NET_ASN_MsgLimitCreation >();            
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Limit::~MIL_Limit()
{
    NET_ASN_MsgLimitDestruction msg;
    msg() = GetID();
    msg.Send();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Limit::load
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_Limit::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_TacticalLine_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::save
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_Limit::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_TacticalLine_ABC >( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Limit::Update
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_Limit::Update( const ASN1T_MsgLimitUpdateRequest& asn )
{
    MIL_TacticalLine_ABC::Update( asn.tactical_line );
    SendMsg< NET_ASN_MsgLimitUpdate >();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename T > void MIL_Limit::SendMsg
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > void MIL_Limit::SendMsg() const
{
    T msg;
    msg().oid = GetID();  
    Serialize( msg().tactical_line );
    msg.Send();
    CleanAfterSerialization( msg().tactical_line );
}

// -----------------------------------------------------------------------------
// Name: MIL_Limit::SendFullState
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_Limit::SendFullState() const
{
    SendMsg< NET_ASN_MsgLimitCreation >();
}
