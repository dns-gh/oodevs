// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SelfTrainingPage.h"
#include "BattleCenterPage.h"
#include "Config.h"
#include "MenuButton.h"
#include "ScenarioLauncherPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::SelfTrainingPage( Q3WidgetStack* pages, Page_ABC& previous, const Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
{
    singleButton_ = AddLink( *new ScenarioLauncherPage( pages, *this, controllers, config, fileLoader, launcher, interpreter ) );
    multiButton_ = AddLink( *new BattleCenterPage( pages, *this, config, fileLoader, controllers, launcher, interpreter ) );
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::~SelfTrainingPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void SelfTrainingPage::OnLanguageChanged()
{
    SetTextAndSubtitle( singleButton_, tools::translate( "SelfTrainingPage", "Single" ), tools::translate( "SelfTrainingPage", "Start a single training session" ) );
    SetTextAndSubtitle( multiButton_, tools::translate( "SelfTrainingPage", "Multiplayer" ), tools::translate( "SelfTrainingPage", "Start or join a multiplayer training session" ) );
    MenuPage::OnLanguageChanged();
}
