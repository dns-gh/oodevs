// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "HomePage.h"
#include "BattleCenterPage.h"
#include "Config.h"
#include "OptionsPage.h"
#include "QuitPage.h"
#include "SelfTrainingPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( QWidgetStack* pages, const Config& config, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter, NetworkExerciseLister& lister )
    : MenuPage( pages )
{
    AddLink( tools::translate( "HomePage", "Self Training" ), *new SelfTrainingPage( pages, *this, config, controllers, interpreter ), true, tools::translate( "HomePage", "Start single player training session" ) );
    AddLink( tools::translate( "HomePage", "Multiplayer" ),   *new BattleCenterPage( pages, *this, config, controllers, lister ), true, tools::translate( "HomePage", "Start or join a multiplayer training session" ) );
    AddLink( tools::translate( "HomePage", "Options" ),       *new OptionsPage( pages, *this ), true, tools::translate( "HomePage", "Options" ) );
    AddLink( tools::translate( "HomePage", "Quit" ),          *new QuitPage( pages, *this ) );
}

// -----------------------------------------------------------------------------
// Name: HomePage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::~HomePage()
{
    // NOTHING
}
