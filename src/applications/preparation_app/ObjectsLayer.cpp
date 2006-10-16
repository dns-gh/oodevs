// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectsLayer.h"
#include "clients_kernel/Object_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::ObjectsLayer( controllers, tools, strategy, view, profile )
    , selected_( controllers )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleKeyPress
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        delete (const kernel::Object_ABC*)selected_;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifySelected
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifySelected( const kernel::Object_ABC* object )
{
    selected_ = object;
    if( object )
        SelectEntity( *object );
}
