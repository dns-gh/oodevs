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
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: BattleCenterPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::BattleCenterPage( QWidgetStack* pages, Page_ABC& previous )
    : ContentPage( pages, tools::translate( "BattleCenterPage", "BattleCenter" ), previous )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BattleCenterPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
BattleCenterPage::~BattleCenterPage()
{
    // NOTHING
}
