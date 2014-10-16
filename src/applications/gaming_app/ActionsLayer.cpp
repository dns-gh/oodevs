// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsLayer.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ActionsLayer constructor
// Created: SBO 2010-04-30
// -----------------------------------------------------------------------------
ActionsLayer::ActionsLayer( kernel::Controllers& controllers, gui::GLView_ABC& tools )
    : gui::Layer( controllers, tools, eLayerTypes_Actions )
    , current_( controllers )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsLayer destructor
// Created: SBO 2010-04-30
// -----------------------------------------------------------------------------
ActionsLayer::~ActionsLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsLayer::Paint
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void ActionsLayer::Paint( gui::Viewport_ABC& viewport )
{
    if( current_ != 0 )
        current_->Draw( viewport, view_ );
}

// -----------------------------------------------------------------------------
// Name: ActionsLayer::NotifySelected
// Created: SBO 2010-04-30
// -----------------------------------------------------------------------------
void ActionsLayer::NotifySelected( const actions::Action_ABC* element )
{
    current_ = element;
}
