// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterEnumeration.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/OrderParameterValue.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

namespace
{
    unsigned long ReadValue( xml::xistream& xis )
    {
        unsigned long value;
        xis >> attribute( "value", value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::ActionParameterEnumeration( const OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , value_( parameter.Get( ReadValue( xis ) ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::ActionParameterEnumeration( const OrderParameter& parameter, unsigned int value )
    : ActionParameter< QString >( parameter )
    , value_( parameter.Get( value ) )
{
    SetValue( value_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterEnumeration::~ActionParameterEnumeration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEnumeration::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterEnumeration::Serialize( xml::xostream& xos ) const
{
    ActionParameter< QString >::Serialize( xos );
    xos << attribute( "value", value_.GetId() );
}
