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
#include "gaming/Publisher_ABC.h"
#include "gaming/SimulationMessages.h"

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
        simulation::ObjectMagicAction asn;
        asn().action.t = T_MsgObjectMagicAction_action_destroy_object;
        asn().action.u.destroy_object = selected_->GetId();
        asn.Send( publisher_ );
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
