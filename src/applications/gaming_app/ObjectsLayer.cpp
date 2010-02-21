// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectsLayer.h"
#include "clients_kernel/Object_ABC.h"
#include "protocol/simulationsenders.h"
#include "protocol/clientsenders.h"

using namespace Common;


// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, Publisher_ABC& publisher )
    : gui::ObjectsLayer( controllers, tools, strategy, view, profile )
    , publisher_( publisher )
    , selected_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::HandleKeyPress
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
bool ObjectsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
    {
        simulation::ObjectMagicAction message;

        message().mutable_action()->set_destroy_object ( selected_->GetId() );
        message.Send( publisher_ );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::NotifySelected
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ObjectsLayer::NotifySelected( const kernel::Object_ABC* object )
{
    selected_ = object;
    gui::ObjectsLayer::NotifySelected( object );
}
