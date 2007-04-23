// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GenObject.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObject::MissionParameter_GenObject( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , data_               ( *asn.value.u.missionGenObject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObject::~MissionParameter_GenObject()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObject::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value               = bNullValue_;
    asn.value.t                  = T_MissionParameter_value_missionGenObject;
    asn.value.u.missionGenObject = new ASN1T_MissionGenObject();
    data_.Send( *asn.value.u.missionGenObject );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObject::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObject::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    GenObject::AsnDelete( *asn.value.u.missionGenObject );
    delete asn.value.u.missionGenObject;
}
