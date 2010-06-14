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
#include "ScenarioLauncherPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::SelfTrainingPage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
{
    AddLink( tools::translate( "SelfTrainingPage", "Single" ), *new ScenarioLauncherPage( pages, *this, controllers, config ), tools::translate( "SelfTrainingPage", "Start a single training session" ) );
    AddLink( tools::translate( "SelfTrainingPage", "Multiplayer" ), *new BattleCenterPage( pages, *this, config, controllers, lister ), tools::translate( "SelfTrainingPage", "Start or join a multiplayer training session" ) );
}

// -----------------------------------------------------------------------------
// Name: SelfTrainingPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
SelfTrainingPage::~SelfTrainingPage()
{
    // NOTHING
}
