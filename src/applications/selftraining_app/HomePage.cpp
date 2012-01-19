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
HomePage::HomePage( QWidget* parent, Q3WidgetStack* pages, Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages, *this, eButtonOptions | eButtonQuit )
    , config_( config )
    , optionsPage_( new OptionsPage( parent, pages, *this, config ) )
{
    adapt_ =   AddLink( *new AuthoringPage( pages, *this, config, controllers ) );
    editPage_ = new ScenarioEditPage( parent, pages, *this, config, fileLoader, controllers, launcher );
    prepare_ = AddLink( *editPage_ );
    play_ =    AddLink( *new SelfTrainingPage( pages, *this, config, fileLoader, controllers, launcher, interpreter ) );
    replay_ =  AddLink( *new ReplayPage( pages, *this , config, fileLoader, controllers, launcher ) );
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
    SetTextAndSubtitle( adapt_,   tools::translate( "HomePage", "Adapt" ),   tools::translate( "HomePage", "Start authoring, terrain generation or terrain workshop" ) );
    SetTextAndSubtitle( prepare_, tools::translate( "HomePage", "Prepare" ), tools::translate( "HomePage", "Edit scenario" ) );
    SetTextAndSubtitle( play_,    tools::translate( "HomePage", "Play" ),    tools::translate( "HomePage", "Start single player or multiplayer training session" ) );
    SetTextAndSubtitle( replay_,  tools::translate( "HomePage", "Replay" ),  tools::translate( "HomePage", "Replay scenario" ) );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: HomePage::Update
// Created: ABR 2011-11-08
// -----------------------------------------------------------------------------
void HomePage::Update()
{
    switch( config_.GetProfile() )
    {
    case Config::eTerrain:
        adapt_->setEnabled( true );
        prepare_->setEnabled( false );
        play_ ->setEnabled( false );
        replay_->setEnabled( false );
        break;
    case Config::eUser:
        adapt_->setEnabled( true );
        prepare_->setEnabled( true );
        play_ ->setEnabled( true );
        replay_->setEnabled( true );
        break;
    case Config::eAdvancedUser:
        adapt_->setEnabled( true );
        prepare_->setEnabled( false );
        play_ ->setEnabled( false );
        replay_->setEnabled( false );
        break;
    case Config::eAdministrator:
        adapt_->setEnabled( true );
        prepare_->setEnabled( true );
        play_ ->setEnabled( true );
        replay_->setEnabled( true );
        break;
    default:
        throw std::exception( "Unknown profile" );
    }
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
    if( prepare_->isEnabled() )
        editPage_->ShowPackageInstallation( package );
}
