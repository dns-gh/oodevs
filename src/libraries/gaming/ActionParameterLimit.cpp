// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimit.h"
#include "ActionParameterLocation.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line )
    : ActionParameter_ABC( name )
{
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, (const ASN1T_Localisation&)line ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location )
    : ActionParameter_ABC( name )
{
    AddParameter( *new ActionParameterLocation( tools::translate( "ActionParameter", "Location" ), converter, location ) );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterLimit::ActionParameterLimit( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter_ABC( ReadName( xis ) )
{
    xis >> start( "parameter" );
    AddParameter( *new ActionParameterLocation( converter, xis ) );
    xis >> end();
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit destructor
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
ActionParameterLimit::~ActionParameterLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameterLimit::Serialize( xml::xostream& xos ) const
{
    ActionParameter_ABC::Serialize( xos );
    xos << attribute( "type", "limit" );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimit::IsContext
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
bool ActionParameterLimit::IsContext() const
{
    return true;
}
