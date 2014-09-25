// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Controllers.h"
#include "OptionsController.h"
#include "Controller.h"
#include "ActionController.h"
#include "LanguageController.h"
#include "ModeController.h"
#include "ContextMenuObserver_ABC.h"
#include <tools/SelectionObserver_ABC.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Controllers constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::Controllers()
    : options_     ( *new OptionsController() )
    , controller_  ( *new Controller() )
    , actions_     ( *new ActionController() )
    , eventActions_( *new ActionController() )
    , modes_       ( *new ModeController() )
    , languages_   ( *new LanguageController() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Controllers destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::~Controllers()
{
    delete &languages_;
    delete &modes_;
    delete &actions_;
    delete &eventActions_;
    delete &controller_;
    delete &options_;
}

// -----------------------------------------------------------------------------
// Name: Controllers::Register
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Register( tools::Observer_ABC& observer )
{
    // We should delete this method and manually register only to the wanted controller.
    // That why languages_ is not registered here.
    options_.Register( observer );
    controller_.Register( observer );
    actions_.Register( observer );
    modes_.Register( observer );
}

// -----------------------------------------------------------------------------
// Name: Controllers::Unregister
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Unregister( tools::Observer_ABC& observer )
{
    options_.Unregister( observer );
    controller_.Unregister( observer );
    actions_.Unregister( observer );
    modes_.Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: Controllers::Update
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void Controllers::Update( tools::Observer_ABC& observer )
{
    Unregister( observer );
    Register  ( observer );
}

// -----------------------------------------------------------------------------
// Name: Controllers::ChangeMode
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void Controllers::ChangeMode( E_Modes newMode )
{
    modes_.ChangeMode( newMode );
    actions_.ChangeMode( newMode );
    actions_.DeselectAll();
}

// -----------------------------------------------------------------------------
// Name: Controllers::LoadOptions
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void Controllers::LoadOptions( E_Modes mode )
{
    modes_.LoadOptions( mode, options_ );
}

// -----------------------------------------------------------------------------
// Name: Controllers::SaveOptions
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void Controllers::SaveOptions( E_Modes mode )
{
    modes_.SaveOptions( mode, options_ );
}

// -----------------------------------------------------------------------------
// Name: Controllers::GetCurrentMode
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
E_Modes Controllers::GetCurrentMode() const
{
    return modes_.GetCurrentMode();
}
