// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Controllers.h"
#include "Options.h"
#include "Controller.h"
#include "ActionController.h"
#include "ModeController.h"
#include "tools/SelectionObserver_ABC.h"
#include "ContextMenuObserver_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Controllers constructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::Controllers()
    : options_   ( *new Options() )
    , controller_( *new Controller() )
    , actions_   ( *new ActionController() )
    , modes_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Controllers destructor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
Controllers::~Controllers()
{
    delete modes_;
    delete &actions_;
    delete &controller_;
    delete &options_;
}

// -----------------------------------------------------------------------------
// Name: Controllers::Register
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void Controllers::Register( tools::Observer_ABC& observer )
{
    options_.Register( observer );
    controller_.Register( observer );
    actions_.Register( observer );
    if( modes_ )
        modes_->Register( observer );
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
    if( modes_ )
        modes_->Unregister( observer );
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
// Name: Controllers::SetModeController
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
void Controllers::SetModeController( ModeController_ABC* modeController )
{
    modes_ = modeController;
}

// -----------------------------------------------------------------------------
// Name: Controllers::ChangeMode
// Created: JSR 2012-05-21
// -----------------------------------------------------------------------------
void Controllers::ChangeMode( int newMode )
{
    if( modes_ )
        modes_->ChangeMode( newMode );
    actions_.ChangeMode( newMode );
    actions_.DeselectAll();
}
