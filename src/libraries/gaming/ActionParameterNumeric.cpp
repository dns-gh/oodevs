// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterNumeric.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::ActionParameterNumeric( const QString& name, float value )
    : ActionParameter< float >( name, value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::ActionParameterNumeric( const kernel::OrderParameter& parameter, float value )
    : ActionParameter< float >( parameter, value )
{
    // NOTHING
}

namespace
{
    float ReadValue( xml::xistream& xis )
    {
        float value;
        xis >> attribute( "value", value );
        return value;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::ActionParameterNumeric( const kernel::OrderParameter& parameter, xml::xistream& xis )
    : ActionParameter< float >( parameter, ReadValue( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterNumeric::~ActionParameterNumeric()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterNumeric::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterNumeric::Serialize( xml::xostream& xos ) const
{
    ActionParameter< float >::Serialize( xos );
    xos << attribute( "value", GetValue() );
}
