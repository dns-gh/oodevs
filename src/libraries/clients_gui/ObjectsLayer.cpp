// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectsLayer.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile )
    : EntityLayer< Object_ABC >( controllers, tools, strategy, view, profile )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: AGE 2006-03-23
// ------------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::ContextMenu
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void ObjectsLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& point, const QPoint& where )
{
    const kernel::Object_ABC& object = static_cast< const kernel::Object_ABC& >( entity );
    controllers_.actions_.ContextMenu( object, entity, point, where );
}
