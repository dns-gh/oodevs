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

using namespace xml;

unsigned long ActionParameter_ABC::idManager_ = 0; // $$$$ SBO 2007-03-12: real id manager maybe

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionParameter_ABC::ActionParameter_ABC( const QString& name, bool context /* = false */ )
    : id_( idManager_++ )
    , name_( name )
    , context_( context )
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
// Name: ActionParameter_ABC::GetId
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
unsigned long ActionParameter_ABC::GetId() const
{
    return id_;
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
// Name: ActionParameter_ABC::SerializeParameter
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameter_ABC::SerializeParameter( xml::xostream& xos ) const
{
    if( !context_ ) // $$$$ SBO 2007-04-24: ...
    {
        xos << start( "parameter" )
                << attribute( "name", name_ )
                << attribute( "type", name_ ); // $$$$ SBO 2007-04-24: get from OrderParameter
        Serialize( xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::SerializeContext
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameter_ABC::SerializeContext( xml::xostream& xos ) const
{
    if( context_ ) // $$$$ SBO 2007-04-24: ...
    {
        xos << start( "parameter" )
                << attribute( "name", name_ )
                << attribute( "type", name_ ); // $$$$ SBO 2007-04-24: get from OrderParameter
        Serialize( xos );
        xos << end();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameter_ABC::Serialize
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionParameter_ABC::Serialize( xml::xostream& xos ) const
{
    // NOTHING
}