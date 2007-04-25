// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomat.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomat constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::ParamAutomat( QObject* parent, const kernel::OrderParameter& parameter )
    : EntityParameter< Automat_ABC >( parent, parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomat::~ParamAutomat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomat::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAutomat::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_automate;
    EntityParameter< Automat_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.automate );
    asn.null_value = asn.value.u.automate ? 0 : 1;
}
