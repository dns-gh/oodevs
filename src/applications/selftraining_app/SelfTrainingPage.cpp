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
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::SelfTrainingPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter )
    : MenuPage( pages )
{
    AddLink( tools::translate( "SelfTrainingPage", "Scenario" ), *new ScenarioPage( pages, *this, config, controllers ), true, tools::translate( "SelfTrainingPage", "Start scenarios" ) );
    AddLink( tools::translate( "SelfTrainingPage", "Tutorials" ), *new TutorialPage( pages, *this, config, controllers, interpreter ), true, tools::translate( "SelfTrainingPage", "Start tutorials" ) );
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
