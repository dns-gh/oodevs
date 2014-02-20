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
#include "clients_kernel/Tools.h"
#include "moc_HomePage.cpp"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( Application& app, QWidget* parent, QStackedWidget* pages,
                    Config& config, const tools::Loader_ABC& fileLoader,
                    kernel::Controllers& controllers,
                    ExerciseContainer& exercises )
    : MenuPage( pages, *this, eButtonAdmin | eButtonQuit )
    , config_( config )
    , optionsPage_( new OptionsPage( app, parent, pages, *this, config, fileLoader, controllers, exercises ) )
{
    setWindowTitle( "HomePage" );

    adaptPage_ = new AuthoringPage( app, parent, pages, *this, config, controllers );
    adapt_ =   AddLink( *adaptPage_ );

    editPage_ = new ScenarioEditPage( app, parent, pages, *this, config, fileLoader, controllers, exercises );
    prepare_ = AddLink( *editPage_, false );
    connect( prepare_, SIGNAL( clicked() ), this, SLOT( OnPrepare() ) );

    playPage_ = new SelfTrainingPage( app, pages, *this, config, fileLoader, controllers, exercises );
    play_ =    AddLink( *playPage_ );

    replayPage_ = new ReplayPage( app, pages, *this , config, fileLoader, controllers, exercises );
    replay_ =  AddLink( *replayPage_, false );
    connect( replay_,  SIGNAL( clicked() ), this, SLOT( OnReplay() ) );
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
    const bool preparation = config_.HasFeature( FEATURE_PREPARATION );
    const bool replayer = config_.HasFeature( FEATURE_REPLAYER );
    SetTextAndSubtitle( adapt_, tools::translate( "HomePage", "Adapt" ),
        tools::translate( "HomePage", "Start authoring, terrain generation" ), false );
    SetTextAndSubtitle( prepare_, tools::translate( "HomePage", "Prepare" ),
        preparation
        ? tools::translate( "HomePage", "Edit scenario" )
        : tools::translate( "HomePage", "Missing Sword-Preparation license" ),
        !preparation );
    SetTextAndSubtitle( play_, tools::translate( "HomePage", "Play" ),
        tools::translate( "HomePage", "Start single player or multiplayer training session" ), false );
    SetTextAndSubtitle( replay_,  tools::translate( "HomePage", "Replay" ),
        replayer
        ? tools::translate( "HomePage", "Replay scenario" )
        : tools::translate( "HomePage", "Missing Sword-Replayer license" ),
        !replayer );
    MenuPage::OnLanguageChanged();
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
void HomePage::InstallPackage( const tools::Path& package )
{
    optionsPage_->ShowPackageInstallation( package );
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnPrepare
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
void HomePage::Update()
{
    OnLanguageChanged();
    adaptPage_->OnLanguageChanged();
    playPage_->OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnPrepare
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
void HomePage::OnPrepare()
{
    if( !config_.HasFeature( FEATURE_PREPARATION ) )
    {
        config_.CheckFeature( FEATURE_PREPARATION );
        Update();
        return;
    }
    editPage_->show();
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnReplay
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
void HomePage::OnReplay()
{
    if( !config_.HasFeature( FEATURE_REPLAYER ) )
    {
        config_.CheckFeature( FEATURE_REPLAYER );
        Update();
        return;
    }
    replayPage_->show();
}
