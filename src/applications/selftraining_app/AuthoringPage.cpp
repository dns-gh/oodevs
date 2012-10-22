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
#include "DataPage.h"
#include "MenuButton.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/StartAuthoring.h"

// -----------------------------------------------------------------------------
// Name: AuthoringPage constructor
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
AuthoringPage::AuthoringPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous,
                              const Config& config, kernel::Controllers& controllers )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_     ( config )
    , controllers_( controllers )
{
    setName( "AuthoringPage" );
    progressPage_ = new ProgressPage( pages, *this );
    dataPage_     = new DataPage( parent, pages, *this, config );

    authoring_       = AddLink( *this, SLOT( OnAuthoring() ) );
    terrainGen_      = AddLink( *new CreateTerrainPage( pages, *this, controllers, config ) );
    data_            = AddLink( *dataPage_ );
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
// Name: AuthoringPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void AuthoringPage::OnLanguageChanged()
{
    progressPage_->SetTitle( tools::translate( "AuthoringPage", "Starting Application" ) );
    SetTextAndSubtitle( authoring_, tools::translate( "AuthoringPage", "Authoring" ),
        config_.hasAuthoring_
        ? tools::translate( "AuthoringPage", "Launch Authoring application" )
        : tools::translate( "AuthoringPage", "Missing Sword-Authoring license" ) );
    SetTextAndSubtitle( terrainGen_, tools::translate( "AuthoringPage", "Terrain Gen" ),
        config_.hasTerrainGeneration_
        ? tools::translate( "AuthoringPage", "Launch Terrain Generation application" )
        : tools::translate( "AuthoringPage", "Missing Sword-Terrain-Generation license" ) );
    SetTextAndSubtitle( data_, tools::translate( "AuthoringPage", "Data" ),
        tools::translate( "AuthoringPage", "Remove Terrains and Models" ) );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::Update
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void AuthoringPage::Update()
{
    authoring_->setEnabled( config_.hasAuthoring_ );
    terrainGen_->setEnabled( config_.hasTerrainGeneration_ );
    dataPage_->SetTerrainsEnabled( true );
    dataPage_->SetModelsEnabled( true );
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
