// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PropertyGroup.h"
#include "RenderingContext_ABC.h"
#include "Property_ABC.h"
#include "PropertyFactory.h"
#include "References_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PropertyGroup constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
PropertyGroup::PropertyGroup( const std::string& css, const PropertyFactory& factory )
{
    factory.CreateProperties( *this, css );
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PropertyGroup::PropertyGroup( xml::xistream& input, const PropertyFactory& factory )
    : class_( xml::attribute( input, "class", std::string() ) )
{
    factory.CreateProperties( *this, input );
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PropertyGroup::PropertyGroup( const PropertyGroup& rhs )
    : class_( rhs.class_ )
{
    for( CIT_Properties it = rhs.properties_.begin(); it != rhs.properties_.end(); ++it )
        properties_.push_back( T_Property( it->first, & it->second->Clone() ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PropertyGroup::~PropertyGroup()
{
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup::Add
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
void PropertyGroup::Add( RenderingContext_ABC::E_Properties name, Property_ABC& property )
{
    properties_.push_back( T_Property( name, &property ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup::Push
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PropertyGroup::Push( RenderingContext_ABC& context, References_ABC& references ) const
{
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        context.PushProperty( it->first, *it->second );
    if( !class_.empty() )
        references.Push( class_, context );
}

// -----------------------------------------------------------------------------
// Name: PropertyGroup::Pop
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
void PropertyGroup::Pop( RenderingContext_ABC& context, References_ABC& references ) const
{
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        context.PopProperty( it->first );
    if( !class_.empty() )
        references.Pop( class_, context );
}
