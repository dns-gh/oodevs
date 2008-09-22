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
#include "ScenarioPage.h"
#include "TutorialPage.h" 
#include "SessionRunningPage.h" 
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::SelfTrainingPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< SessionStatus > sessionStatus )
    : MenuPage( pages )
{
    SessionRunningPage* running = new SessionRunningPage( pages, *this, config, sessionStatus ) ; 
    AddLink( tools::translate( "SelfTrainingPage", "Scenario" ), *new ScenarioPage( pages, *this, config, *running, sessionStatus ), true, tools::translate( "SelfTrainingPage", "Start scenarios" ) );
    AddLink( tools::translate( "SelfTrainingPage", "Tutorials" ), *new TutorialPage( pages, *this, *running, config, interpreter, sessionStatus ), true, tools::translate( "SelfTrainingPage", "Start tutorials" ) );
    AddLink( tools::translate( "SelfTrainingPage", "Back" ), previous );
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::~SelfTrainingPage()
{
    // NOTHING
}
