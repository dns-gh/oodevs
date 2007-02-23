// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Drawer.h"
#include "Drawable_ABC.h"
#include "Extension_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Drawer constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Drawer::Drawer()
{
    SetPosition( "Lives",      0 );
    SetPosition( "Positions",  1 );
    SetPosition( "Object",     2 );
    SetPosition( "Agent",      2 );
    SetPosition( "Attributes", 3 );
}

// -----------------------------------------------------------------------------
// Name: Drawer destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Drawer::~Drawer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawer::SetPosition
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::SetPosition( const std::string& name, unsigned pos )
{
    positions_[ name ] = pos;
}

// -----------------------------------------------------------------------------
// Name: Drawer::Register
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::Register( const Extension_ABC& extension )
{
    const Drawable_ABC* drawable = dynamic_cast< const Drawable_ABC* >( &extension );
    if( drawable )
        Add( *drawable );
}

// -----------------------------------------------------------------------------
// Name: Drawer::Add
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::Add( const Drawable_ABC& extension )
{
    const std::string name = Strip( typeid( extension ).name() );
    CIT_Positions it = positions_.find( name );
    if( it != positions_.end() && it->second < extensions_.size() )
        extensions_.insert( extensions_.begin() + it->second, &extension );
    else
        extensions_.push_back( &extension );
}

// -----------------------------------------------------------------------------
// Name: Drawer::Strip
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
std::string Drawer::Strip( const std::string& name )
{
    std::string::size_type pos = name.find_last_of( ' ' );
    if( pos != std::string::npos )
        return name.substr( pos + 1 );
    return name;
}

// -----------------------------------------------------------------------------
// Name: Drawer::Draw
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    for( CIT_Drawables it = extensions_.begin(); it != extensions_.end(); ++it )
        (*it)->Draw( where, viewport, tools );
}
