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
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterLauncherPage constructor
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
BattleCenterLauncherPage::BattleCenterLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ScenarioLauncherPage( pages, previous, controllers, config, tools::translate( "BattleCenterLauncherPage", "Multiplayer session" ) )
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
