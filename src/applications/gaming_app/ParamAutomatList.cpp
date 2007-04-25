// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomatList.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomatList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::ParamAutomatList( QObject* parent, const kernel::OrderParameter& parameter )
    : EntityListParameter< Automat_ABC >( parent, parameter )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAutomatList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::~ParamAutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAutomatList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listAutomate;
    EntityListParameter< Automat_ABC >::CommitTo( (ASN1T_ListOID*&)asn.value.u.listAutomate );
    asn.null_value = asn.value.u.listAutomate->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAutomatList::Clean( ASN1T_MissionParameter& asn ) const
{
    EntityListParameter< Automat_ABC >::Clean( (ASN1T_ListOID*&)asn.value.u.listAutomate );
}
