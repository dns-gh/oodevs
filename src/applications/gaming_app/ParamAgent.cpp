// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgent.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgent constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::ParamAgent( QObject* parent, const QString& name )
    : EntityParameter< Agent_ABC >( parent, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgent::~ParamAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgent::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgent::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_agent;
    EntityParameter< Agent_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.agent );
    asn.null_value = asn.value.u.agent ? 0 : 1;
}
