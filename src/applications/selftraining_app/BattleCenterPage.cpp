// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "BattleCenterPage.h"
#include "BattleCenterLauncherPage.h"
#include "BattleCenterJoinPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::BattleCenterPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, NetworkExerciseLister& lister )
    : MenuPage( pages )
{
    AddLink( tools::translate( "BattleCenterPage", "Start" ), *new BattleCenterLauncherPage( pages, *this, controllers, config ), true, tools::translate( "ScenarioPage", "Start multiplayer training session" ) );
    AddLink( tools::translate( "BattleCenterPage", "Join" ),  *new BattleCenterJoinPage( pages, *this, controllers, config, lister ), true, tools::translate( "ScenarioPage", "Join multiplayer training session" ) );
    AddLink( tools::translate( "BattleCenterPage", "Back" ),  previous );
}

// -----------------------------------------------------------------------------
// Name: BattleCenterPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::~BattleCenterPage()
{
    // NOTHING
}
