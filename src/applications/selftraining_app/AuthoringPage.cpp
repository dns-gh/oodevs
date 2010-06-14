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
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "ProcessWrapper.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/StartAuthoring.h" 
#include "frontend/StartTerrainWorkshop.h" 

// -----------------------------------------------------------------------------
// Name: AuthoringPage constructor
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
AuthoringPage::AuthoringPage( QWidgetStack* pages, Page_ABC& previous, const Config& config, kernel::Controllers& controllers )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "AuthoringPage", "Starting Application" ), controllers ) )
{
    AddLink( tools::translate( "AuthoringPage", "Authoring" ), *this, tools::translate( "AuthoringPage", "Launch Authoring application" ), SLOT( OnAuthoring() ) );
    AddLink( tools::translate( "AuthoringPage", "Terrain Gen" ), *new CreateTerrainPage( pages, *this, controllers, config ), tools::translate( "AuthoringPage", "Launch Terrain Generation application" ) );
    AddLink( tools::translate( "AuthoringPage", "Terrain Workshop" ), *this, tools::translate( "AuthoringPage", "Launch Terrain Workshop application" ), SLOT( OnTerrainWorkshop() ) );
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
// Name: AuthoringPage::OnAuthoring
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void AuthoringPage::OnAuthoring()
{
    if( !dialogs::KillRunningProcesses( this ) )
        return;

    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::StartAuthoring( config_, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
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
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}
