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
#include "MenuButton.h"
#include "RemoteControlPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::BattleCenterPage( Q3WidgetStack* pages, Page_ABC& previous, const Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
{
    bcLauncher_ =     AddLink( *new BattleCenterLauncherPage( pages, *this, controllers, config, fileLoader, launcher, interpreter ) );
    bcJoin_ =         AddLink( *new BattleCenterJoinPage( pages, *this, controllers, config, fileLoader, launcher ) );
    remoteControle_ = AddLink( *new RemoteControlPage( pages, *this, controllers, config, fileLoader, launcher ) );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::~BattleCenterPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void BattleCenterPage::OnLanguageChanged()
{
    SetTextAndSubtitle( bcLauncher_,     tools::translate( "BattleCenterPage", "Start" ),  tools::translate( "ScenarioPage", "Start multiplayer training session" ) );
    SetTextAndSubtitle( bcJoin_,         tools::translate( "BattleCenterPage", "Join" ),   tools::translate( "ScenarioPage", "Join multiplayer training session" ) );
    SetTextAndSubtitle( remoteControle_, tools::translate( "BattleCenterPage", "Remote" ), tools::translate( "ScenarioPage", "Control remote training sessions" ) );
    MenuPage::OnLanguageChanged();
}
