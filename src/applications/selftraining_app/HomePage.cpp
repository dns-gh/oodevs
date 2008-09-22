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
#include "SelfTrainingPage.h"
#include "BattleCenterPage.h"
#include "OptionsPage.h"
#include "QuitPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( QWidgetStack* pages, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< SessionStatus > sessionStatus )
    : MenuPage( pages )
{
    AddLink( tools::translate( "HomePage", "Self Training" ), *new SelfTrainingPage( pages, *this, config, interpreter, sessionStatus ), true, tools::translate( "HomePage", "Start Exercises" ) );
    AddLink( tools::translate( "HomePage", "Multiplayer" ),   *new BattleCenterPage( pages, *this ), false, tools::translate( "HomePage", "Start battlecenter mode " ) );
    AddLink( tools::translate( "HomePage", "Options" ),       *new OptionsPage( pages, *this ), false, tools::translate( "HomePage", "Options" ) );
    AddLink( tools::translate( "HomePage", "Quit" ),          *new QuitPage( pages, *this ) );
}

// -----------------------------------------------------------------------------
// Name: HomePage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::~HomePage()
{
    // NOTHING
}
