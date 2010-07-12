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
#include "Config.h"
#include "ImportPage.h" // LTO
#include "MenuButton.h"
#include "ReplayPage.h"
#include "ScenarioEditPage.h"
#include "SelfTrainingPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_( config )
{
    adapt_ = AddLink( tools::translate( "HomePage", "Adapt" ), *new AuthoringPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Start authoring, terrain generation or terrain workshop" ) );
    import_ = AddLink( tools::translate( "HomePage", "Import" ), *new ImportPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Importer depuis l'editeur de scenario" ) ); // LTO
    prepare_ = AddLink( tools::translate( "HomePage", "Prepare" ), *new ScenarioEditPage( pages, *this, config, controllers ), tools::translate( "HomePage", "Edit scenario" ) );
    play_ = AddLink( tools::translate( "HomePage", "Play" ), *new SelfTrainingPage( pages, *this, config, controllers, lister ), tools::translate( "HomePage", "Start single player or multiplayer training session" ) );
    replay_ = AddLink( tools::translate( "HomePage", "Replay" ), *new ReplayPage( pages, *this , controllers, config ), tools::translate( "HomePage", "Replay scenario" ) );
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
// Name: HomePage::show
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void HomePage::show()
{
    switch( config_.GetProfile() )
    {
    case Config::eTerrain:
        adapt_->setEnabled( true );
        import_->setEnabled( false );
        prepare_->setEnabled( false );
        play_ ->setEnabled( false );
        replay_->setEnabled( false );
        break;
    case Config::eUser:
        adapt_->setEnabled( true );
        import_->setEnabled( true );
        prepare_->setEnabled( true );
        play_ ->setEnabled( true );
        replay_->setEnabled( true );
        break;
    case Config::eAdvancedUser:
        adapt_->setEnabled( true );
        import_->setEnabled( true );
        prepare_->setEnabled( false );
        play_ ->setEnabled( false );
        replay_->setEnabled( false );
        break;
    case Config::eAdministrator:
        adapt_->setEnabled( true );
        import_->setEnabled( true );
        prepare_->setEnabled( true );
        play_ ->setEnabled( true );
        replay_->setEnabled( true );
        break;
    default:
        throw std::exception( "Unknown profile" );
    }

    MenuPage::show();
}
