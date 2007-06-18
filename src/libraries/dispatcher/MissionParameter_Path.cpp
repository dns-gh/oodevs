// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Path.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Path::MissionParameter_Path( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , path_               ( *asn.value.u.path )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Path::~MissionParameter_Path()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Path::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value         = bNullValue_;
    asn.value.t            = T_MissionParameter_value_path;
    asn.value.u.path = new ASN1T_Path();
    path_.Send( *asn.value.u.location );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Path::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Path::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    Localisation::AsnDelete( *asn.value.u.path );
    delete asn.value.u.path;
}
