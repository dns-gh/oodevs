// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterEntityList.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "ActionParameterEntity.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const kernel::OrderParameter& parameter, const ASN1T_ListAgent& asn, const kernel::Resolver_ABC< kernel::Agent_ABC >& agents )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterEntity< kernel::Agent_ABC >( tools::translate( "ActionParameter", "Agent %1" ).arg( i ), &agents.Get( asn.elem[i] ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const kernel::OrderParameter& parameter, const ASN1T_ListAutomate& asn, const kernel::Resolver_ABC< kernel::Automat_ABC >& automats )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterEntity< kernel::Automat_ABC >( tools::translate( "ActionParameter", "Automat %1" ).arg( i ), &automats.Get( asn.elem[i] ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList destructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::~ActionParameterEntityList()
{
    // NOTHING
}
