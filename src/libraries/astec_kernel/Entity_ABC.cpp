// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "Entity_ABC.h"
#include "Drawable_ABC.h"
#include "Drawer.h"

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
    delete drawer_;
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
    Register( ext );
    drawer_->Register( ext );
    Extendable< Extension_ABC >::AddExtension( ext );
}

// -----------------------------------------------------------------------------
// Name: Entity_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void Entity_ABC::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    drawer_->Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: InterfaceContainer< Extension_ABC >& Entity_ABC::Interface
// Created: AGE 2006-05-02
// -----------------------------------------------------------------------------
InterfaceContainer< Extension_ABC >& Entity_ABC::Interface() const
{
    return *const_cast< Entity_ABC* >( this );
}
