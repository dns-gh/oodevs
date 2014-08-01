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
#include "clients_kernel/Tools.h"
#include "frontend/DebugConfig.h"
#include "moc_SelfTrainingPage.cpp"

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::SelfTrainingPage( Application& app, QStackedWidget* pages,
                                    Page_ABC& previous, const Config& config,
                                    const tools::Loader_ABC& fileLoader,
                                    kernel::Controllers& controllers,
                                    ExerciseContainer& exercises,
                                    const frontend::DebugConfig& debug )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_( config )
{
    setWindowTitle( "SelfTrainingPage" );
    launcher_ = new ScenarioLauncherPage( app, pages, *this, controllers, config,
            fileLoader, exercises, debug );
    startButton_ = AddLink( *launcher_, false );
    connect( startButton_, SIGNAL( clicked() ), this, SLOT( OnStart() ) );
    joinButton_ = AddLink( *new ScenarioJoinPage( app, pages, *this, controllers, config, fileLoader, exercises, debug ) );
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
    const bool runtime = config_.HasFeature( FEATURE_RUNTIME );
    SetTextAndSubtitle( startButton_, tools::translate( "SelfTrainingPage", "Start" ),
        runtime
        ? tools::translate( "SelfTrainingPage", "Start a training session" )
        : tools::translate( "SelfTrainingPage", "Missing Sword-Runtime license" ),
        !runtime );
    SetTextAndSubtitle( joinButton_, tools::translate( "SelfTrainingPage", "Join" ),
        tools::translate( "SelfTrainingPage", "Join a remote training session" ), false );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage::OnStart
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
void SelfTrainingPage::OnStart()
{
    if( !config_.HasFeature( FEATURE_RUNTIME ) )
    {
        config_.CheckFeature( FEATURE_RUNTIME );
        OnLanguageChanged();
        return;
    }
    launcher_->show();
}
