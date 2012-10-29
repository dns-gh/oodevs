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
#include "AuthoringPage.h"
#include "Config.h"
#include "MenuButton.h"
#include "OptionsPage.h"
#include "ReplayPage.h"
#include "ScenarioEditPage.h"
#include "SelfTrainingPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( Application& app, QWidget* parent, Q3WidgetStack* pages, Config& config,
                    const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers,
                    frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages, *this, eButtonAdmin | eButtonQuit )
    , config_( config )
    , optionsPage_( new OptionsPage( app, parent, pages, *this, config, fileLoader, controllers, launcher ) )
{
    setName( "HomePage" );
    adapt_ =   AddLink( *new AuthoringPage( app, parent, pages, *this, config, controllers ) );
    editPage_ = new ScenarioEditPage( app, parent, pages, *this, config, fileLoader, controllers, launcher );
    prepare_ = AddLink( *editPage_ );
    play_ =    AddLink( *new SelfTrainingPage( app, pages, *this, config, fileLoader, controllers, launcher, interpreter ) );
    replay_ =  AddLink( *new ReplayPage( app, pages, *this , config, fileLoader, controllers, launcher ) );
}

// -----------------------------------------------------------------------------
// Name: HomePage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::~HomePage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void HomePage::OnLanguageChanged()
{
    SetTextAndSubtitle( adapt_, tools::translate( "HomePage", "Adapt" ),
        tools::translate( "HomePage", "Start authoring, terrain generation or terrain workshop" ) );
    SetTextAndSubtitle( prepare_, tools::translate( "HomePage", "Prepare" ),
        config_.hasPreparation_
        ? tools::translate( "HomePage", "Edit scenario" )
        : tools::translate( "HomePage", "Missing Sword-Preparation license" ) );
    SetTextAndSubtitle( play_, tools::translate( "HomePage", "Play" ),
        tools::translate( "HomePage", "Start single player or multiplayer training session" ) );
    SetTextAndSubtitle( replay_,  tools::translate( "HomePage", "Replay" ),
        config_.hasReplayer_
        ? tools::translate( "HomePage", "Replay scenario" )
        : tools::translate( "HomePage", "Missing Sword-Replayer license" ) );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: HomePage::Update
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void HomePage::Update()
{
    adapt_->setEnabled( true );
    prepare_->setEnabled( config_.hasPreparation_ );
    play_->setEnabled( true );
    replay_->setEnabled( config_.hasReplayer_ );
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnOptions
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
void HomePage::OnOptions()
{
    optionsPage_->show();
}

// -----------------------------------------------------------------------------
// Name: HomePage::InstallPackage
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void HomePage::InstallPackage( const QString& package )
{
    optionsPage_->ShowPackageInstallation( package );
}
