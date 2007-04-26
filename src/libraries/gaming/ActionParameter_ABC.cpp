// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameter_ABC.h"
#include "xeumeuleu/xml.h"
#include "Tools.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::ActionParameter_ABC( const QString& name )
    : name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::~ActionParameter_ABC()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::GetName
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
QString ActionParameter_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Draw
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "name", name_ );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "parameter" );
        it->second->Serialize( xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::AddParameter
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ActionParameter_ABC::AddParameter( ActionParameter_ABC& parameter )
{
    Register( Count(), parameter );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::IsContext
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ActionParameter_ABC::IsContext() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Display
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "ActionParameter", "Value" ) ).Display( "" );
}
