// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "BattleCenterPage.h"
#include "BattleCenterLauncherPage.h"
#include "BattleCenterJoinPage.h"
#include "Config.h"
#include "RemoteControlPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::BattleCenterPage( Q3WidgetStack* pages, Page_ABC& previous, const Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
{
    AddLink( tools::translate( "BattleCenterPage", "Start" ), *new BattleCenterLauncherPage( pages, *this, controllers, config, fileLoader, launcher, interpreter ), tools::translate( "ScenarioPage", "Start multiplayer training session" ) );
    AddLink( tools::translate( "BattleCenterPage", "Join" ),  *new BattleCenterJoinPage( pages, *this, controllers, config, fileLoader, launcher ), tools::translate( "ScenarioPage", "Join multiplayer training session" ) );
    AddLink( tools::translate( "BattleCenterPage", "Remote" ),  *new RemoteControlPage( pages, *this, controllers, config, fileLoader, launcher ), tools::translate( "ScenarioPage", "Control remote training sessions" ) );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::~BattleCenterPage()
{
    // NOTHING
}
