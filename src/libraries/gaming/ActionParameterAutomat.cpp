// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterAutomat.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/Automat_ABC.h"

using namespace kernel;

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( const QString& name )
    : ActionParameterEntity< Automat_ABC >( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( const kernel::OrderParameter& parameter )
    : ActionParameterEntity< Automat_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Automat_ABC >& resolver )
    : ActionParameterEntity< Automat_ABC >( parameter, &resolver.Get( ReadId( xis ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( const QString& name, unsigned int id, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver )
    : ActionParameterEntity< Automat_ABC >( name, &resolver.Get( id ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver )
    : ActionParameterEntity< Automat_ABC >( parameter, &resolver.Get( id ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
ActionParameterAutomat::ActionParameterAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver )
    : ActionParameterEntity< Automat_ABC >( ReadName( xis ), &resolver.Get( ReadId( xis ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat destructor
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
ActionParameterAutomat::~ActionParameterAutomat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterAutomat::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_automate;
    ActionParameterEntity< Automat_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.automate );
    asn.null_value = asn.value.u.automate ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat::Accept
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAutomat::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterAutomat::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ActionParameterAutomat::CommitTo( ASN1T_Automate& asn ) const
{
    ActionParameterEntity< Automat_ABC >::CommitTo( (ASN1T_OID&)asn );
}
