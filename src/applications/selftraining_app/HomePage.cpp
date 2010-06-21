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
#include "AuthoringPage.h"
#include "ImportPage.h" // LTO
#include "ReplayPage.h"
#include "ScenarioEditPage.h"
#include "SelfTrainingPage.h"
#include "Config.h"
#include "OptionsPage.h"
#include "QuitPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( QWidgetStack* pages, const Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister )
    : MenuPage( pages, *this, eButtonOptions | eButtonQuit )
{
    AddLink( tools::translate( "HomePage", "Adapt" ), *new AuthoringPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Start authoring, terrain generation or terrain workshop" ) );
    AddLink( tools::translate( "HomePage", "Import" ), *new ImportPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Importer depuis l'editeur de scenario" ) ); // LTO
    AddLink( tools::translate( "HomePage", "Prepare" ), *new ScenarioEditPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Edit scenario" ) );
    AddLink( tools::translate( "HomePage", "Play" ), *new SelfTrainingPage( pages, *this, config, controllers, lister ), tools::translate( "HomePage", "Start single player or multiplayer training session" ) );
    AddLink( tools::translate( "HomePage", "Replay" ), *new ReplayPage( pages, *this , controllers, config ), tools::translate( "HomePage", "Replay scenario" ) );

    optionsPage_ = new OptionsPage( pages, *this, config ), true, tools::translate( "HomePage", "Options" );
}

// -----------------------------------------------------------------------------
// Name: HomePage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::~HomePage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HomePage::OnOptions
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
void HomePage::OnOptions()
{
    optionsPage_->show();
}
