// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Lima.h"

#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_Lima, "MIL_Lima" )

// -----------------------------------------------------------------------------
// Name: MIL_Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Lima::MIL_Lima()
    : MIL_TacticalLine_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Lima::MIL_Lima( const ASN1T_MsgLimaCreationRequest& asn )
    : MIL_TacticalLine_ABC( asn )
{
    SendMsg< NET_ASN_MsgLimaCreation >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Lima::MIL_Lima( const MIL_Automate& automateBroadcasted, MIL_InputArchive& archive )
    : MIL_TacticalLine_ABC( automateBroadcasted, archive )
{
    SendMsg< NET_ASN_MsgLimaCreation >();
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Lima::MIL_Lima( const MIL_Formation& formationBroadcasted, MIL_InputArchive& archive )
    : MIL_TacticalLine_ABC( formationBroadcasted, archive )
{
    SendMsg< NET_ASN_MsgLimaCreation >();            
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
MIL_Lima::~MIL_Lima()
{
    NET_ASN_MsgLimaDestruction msg;
    msg() = GetID();
    msg.Send();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Lima::load
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_Lima::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_TacticalLine_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima::save
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_Lima::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_TacticalLine_ABC >( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Lima::Update
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_Lima::Update( const ASN1T_MsgLimaUpdateRequest& asn )
{
    MIL_TacticalLine_ABC::Update( asn.tactical_line );
    SendMsg< NET_ASN_MsgLimaUpdate >();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename T > void MIL_Lima::SendMsg
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > void MIL_Lima::SendMsg() const
{
    T msg;
    msg().oid = GetID();  
    Serialize( msg().tactical_line );
    msg.Send();
    CleanAfterSerialization( msg().tactical_line );
}

// -----------------------------------------------------------------------------
// Name: MIL_Lima::SendFullState
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
void MIL_Lima::SendFullState() const
{
    SendMsg< NET_ASN_MsgLimaCreation >();
}
