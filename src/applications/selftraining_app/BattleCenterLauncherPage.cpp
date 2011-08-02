// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "BattleCenterLauncherPage.h"
#include "moc_BattleCenterLauncherPage.cpp"
#include "Multiplayer.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "clients_gui/Tools.h"
#include "frontend/CreateSession.h"
#include "frontend/Config.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/JoinExercise.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/PluginConfig_ABC.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
BattleCenterLauncherPage::BattleCenterLauncherPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const frontend::Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : ScenarioLauncherPage( pages, previous, controllers, config, fileLoader, launcher, interpreter, tools::translate( "BattleCenterLauncherPage", "Multiplayer session" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage destructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
BattleCenterLauncherPage::~BattleCenterLauncherPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage::BuildSessionName
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
std::string BattleCenterLauncherPage::BuildSessionName() const
{
    return MULTIPLAYER_SESSION;
}

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage::OnStart
// Created: HBD 2011-03-04
// -----------------------------------------------------------------------------
void BattleCenterLauncherPage::OnStart()
{
    if( !CanBeStarted() || ! dialogs::KillRunningProcesses( this ) )
        return;
    exercise_->Start( MULTIPLAYER_SESSION );
    const std::string exerciseName = exercise_->GetName();
    {
        frontend::CreateSession action( config_, exerciseName, MULTIPLAYER_SESSION );
        action.SetDefaultValues();
        action.SetOption( "session/config/gaming/network/@server", QString( "%1:%2" ).arg( "127.0.0.1" ).arg( exercise_->GetPort() ) );
        action.Commit();
    }
    {
        for( T_Plugins::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it )
            (*it)->Commit( exerciseName, MULTIPLAYER_SESSION );
    }
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::JoinExercise( config_, exercise_->GetName().c_str(), MULTIPLAYER_SESSION.c_str(), profile_.GetLogin(), true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}
