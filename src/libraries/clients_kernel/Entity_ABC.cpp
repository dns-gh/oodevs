// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Entity_ABC.h"
#include "Drawable_ABC.h"
#include "Drawer.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Entity_ABC constructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::Entity_ABC()
    : drawer_( new Drawer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC destructor
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
Entity_ABC::~Entity_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString Entity_ABC::GetTypeName() const
{
    return "entity";
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::RegisterSelf
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Entity_ABC::RegisterSelf( Extension_ABC& ext )
{
    AddExtension( ext );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::AddExtension
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
void Entity_ABC::AddExtension( Extension_ABC& ext )
{
    tools::SortedInterfaceContainer< Extension_ABC >::Register( ext );
    drawer_->Register( ext );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Entity_ABC::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    drawer_->Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Interface
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
tools::SortedInterfaceContainer< Extension_ABC >& Entity_ABC::Interface() const
{
    return *const_cast< Entity_ABC* >( this );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::CheckUpdate
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
void Entity_ABC::CheckUpdate( const type_info& /*type*/ )
{
    /*
    throw std::runtime_error( "Nothing in " + std::string( typeid( *this ).name() )
                            + " could be updated with message type " + type );
    */
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::OverFly
// Created: ABR 2011-10-27
// -----------------------------------------------------------------------------
void Entity_ABC::OverFly( ActionController& /*controller*/ ) const
{
    // NOTHING
}
