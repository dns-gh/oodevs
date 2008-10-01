// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioPage.h"
#include "ScenarioLauncherPage.h"
#include "ScenarioEditPage.h" 
#include "TutorialPage.h" 
#include "ReplayPage.h"
#include "SessionRunningPage.h" 
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ScenarioPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioPage::ScenarioPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, SessionRunningPage& running,  boost::shared_ptr< Session > sessionStatus )
    : MenuPage( pages )
{
    AddLink( tools::translate( "ScenarioPage", "Play" ),   *new ScenarioLauncherPage( pages, *this, controllers, running, config, sessionStatus ), true, tools::translate( "ScenarioPage", "Start scenarios" ) );
    AddLink( tools::translate( "ScenarioPage", "Edit" ),   *new ScenarioEditPage( pages, *this, config, controllers, sessionStatus ),              true, tools::translate( "ScenarioPage", "Edit scenario" ) );
    AddLink( tools::translate( "ScenarioPage", "Replay" ), *new ReplayPage( pages, *this , controllers, config, running, sessionStatus ),                   true, tools::translate( "ScenarioPage", "Replay scenario" ) );
    AddLink( tools::translate( "ScenarioPage", "Back" ),   previous );
}

// -----------------------------------------------------------------------------
// Name: ScenarioPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioPage::~ScenarioPage()
{
    // NOTHING
}
