// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAgentList.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAgentList constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentList::ParamAgentList( QObject* parent, const QString& name )
    : EntityListParameter< Agent_ABC >( parent, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
ParamAgentList::~ParamAgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listAgent;
    EntityListParameter< Agent_ABC >::CommitTo( (ASN1T_ListOID*&)asn.value.u.listAgent );
    asn.null_value = asn.value.u.listAgent->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamAgentList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAgentList::Clean( ASN1T_MissionParameter& asn ) const
{
    EntityListParameter< Agent_ABC >::Clean( (ASN1T_ListOID*&)asn.value.u.listAgent );
}
