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
#include "ScenarioJoinPage.h"
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
    , config_( config )
{
    setName( "SelfTrainingPage" );
    startButton_ = AddLink( *new ScenarioLauncherPage( pages, *this, controllers, config, fileLoader, launcher, interpreter ) );
    joinButton_ = AddLink( *new ScenarioJoinPage( pages, *this, controllers, config, fileLoader, launcher ) );
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
    SetTextAndSubtitle( startButton_, tools::translate( "SelfTrainingPage", "Start" ),
        config_.hasRuntime_
        ? tools::translate( "SelfTrainingPage", "Start a training session" )
        : tools::translate( "SelfTrainingPage", "Missing Sword-Runtime license" ) );
    SetTextAndSubtitle( joinButton_, tools::translate( "SelfTrainingPage", "Join" ),
        tools::translate( "SelfTrainingPage", "Join a remote training session" ) );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage::Update
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void SelfTrainingPage::Update()
{
    startButton_->setEnabled( config_.hasRuntime_ );
}