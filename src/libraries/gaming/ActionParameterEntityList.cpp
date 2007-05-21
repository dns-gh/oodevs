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
#include "ActionParameterEntity.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const OrderParameter& parameter, const ASN1T_ListAgent& asn, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterEntity< Agent_ABC >( tools::translate( "ActionParameter", "Agent %1" ).arg( i ), &resolver.Get( asn.elem[i] ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const OrderParameter& parameter, const ASN1T_ListAutomate& asn, const Resolver_ABC< Automat_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < asn.n; ++i )
        AddParameter( *new ActionParameterEntity< Automat_ABC >( tools::translate( "ActionParameter", "Automat %1" ).arg( i ), &resolver.Get( asn.elem[i] ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Agent_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterEntityList::ReadAgent, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEntityList::ActionParameterEntityList( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver )
    : ActionParameter< QString >( parameter )
{
    xis >> list( "parameter", *this, &ActionParameterEntityList::ReadAutomat, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList destructor
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
ActionParameterEntityList::~ActionParameterEntityList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList::ReadAgent
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterEntityList::ReadAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver )
{
    ReadEntity( xis, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList::ReadAutomat
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterEntityList::ReadAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver )
{
    ReadEntity( xis, resolver );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntityList::ReadEntity
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntityList::ReadEntity( xml::xistream& xis, const Resolver_ABC< ConcreteEntity >& resolver )
{
    AddParameter( *new ActionParameterEntity< ConcreteEntity >( xis, resolver ) );
}
