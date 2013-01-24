// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Drawer.h"

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

#include <map>
#include <boost/assign/list_of.hpp>

using namespace gui;

namespace
{
    const std::map< std::string, unsigned > positions = boost::assign::map_list_of
        ( "Lives",      0 )
        ( "Positions",  1 )
        ( "Object",     2 )
        ( "Agent",      2 )
        ( "Attributes", 3 );
}

// -----------------------------------------------------------------------------
// Name: Drawer constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Drawer::Drawer()
{
    // NOTHING
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
// Name: Drawer::Register
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::Register( const kernel::Extension_ABC& extension )
{
    auto drawable = dynamic_cast< const kernel::Drawable_ABC* >( &extension );
    if( drawable )
        Add( *drawable );
}

// -----------------------------------------------------------------------------
// Name: Drawer::Add
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Drawer::Add( const kernel::Drawable_ABC& extension )
{
    const std::string name = Strip( typeid( extension ).name() );
    auto it = positions.find( name );
    if( it != positions.end() && it->second < extensions_.size() )
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
void Drawer::Draw( const geometry::Point2f& where,
                   const kernel::Viewport_ABC& viewport,
                   const kernel::GlTools_ABC& tools ) const
{
    for( auto it = extensions_.begin(); it != extensions_.end(); ++it )
        (*it)->Draw( where, viewport, tools );
}