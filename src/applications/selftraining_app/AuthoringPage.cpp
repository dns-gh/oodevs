// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "AuthoringPage.h"
#include "moc_AuthoringPage.cpp"
#include "Config.h"
#include "CreateTerrainPage.h"
#include "MenuButton.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/StartAuthoring.h"
#include "frontend/StartTerrainWorkshop.h"

// -----------------------------------------------------------------------------
// Name: AuthoringPage constructor
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
AuthoringPage::AuthoringPage( Q3WidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "AuthoringPage", "Starting Application" ) ) )
{
    authoring_ = AddLink( tools::translate( "AuthoringPage", "Authoring" ), *this, tools::translate( "AuthoringPage", "Launch Authoring application" ), SLOT( OnAuthoring() ) );
    terrainGen_ = AddLink( tools::translate( "AuthoringPage", "Terrain Gen" ), *new CreateTerrainPage( pages, *this, controllers, config ), tools::translate( "AuthoringPage", "Launch Terrain Generation application" ) );
    terrainWorkshop_ = AddLink( tools::translate( "AuthoringPage", "Terrain Workshop" ), *this, tools::translate( "AuthoringPage", "Launch Terrain Workshop application" ), SLOT( OnTerrainWorkshop() ) );
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage destructor
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
AuthoringPage::~AuthoringPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::show
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void AuthoringPage::show()
{
    switch( config_.GetProfile() )
    {
    case Config::eTerrain:
        authoring_->setEnabled( false );
        terrainGen_->setEnabled( true );
        terrainWorkshop_->setEnabled( true );
        break;
    case Config::eUser:
    case Config::eAdvancedUser:
        authoring_->setEnabled( true );
        terrainGen_->setEnabled( false );
        terrainWorkshop_->setEnabled( false );
        break;
    case Config::eAdministrator:
        authoring_->setEnabled( true );
        terrainGen_->setEnabled( true );
        terrainWorkshop_->setEnabled( true );
        break;
    default:
        throw std::exception( "Unknown profile" );
    }
    MenuPage::show();
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::OnAuthoring
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void AuthoringPage::OnAuthoring()
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::StartAuthoring( config_, true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::OnTerrainWorkshop
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void AuthoringPage::OnTerrainWorkshop()
{
    if( !dialogs::KillRunningProcesses( this ) )
        return;
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::StartTerrainWorkshop( config_, true ) );
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}
