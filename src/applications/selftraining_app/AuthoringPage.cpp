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
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/StartAuthoring.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: AuthoringPage constructor
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
AuthoringPage::AuthoringPage( Application& app, QWidget* /*parent*/,
                              QStackedWidget* pages, Page_ABC& previous,
                              const Config& config, kernel::Controllers& controllers )
    : MenuPage( pages, previous, eButtonBack | eButtonQuit )
    , config_     ( config )
    , controllers_( controllers )
{
    setWindowTitle( "AuthoringPage" );
    progressPage_ = new ProgressPage( app, pages, *this );

    authoring_ = AddLink( *this, false );
    connect( authoring_, SIGNAL( clicked() ), this, SLOT( OnAuthoring() ) );

    create_ = new CreateTerrainPage( app, pages, *this, controllers, config );
    terrainGen_ = AddLink( *create_, false );
    connect( terrainGen_, SIGNAL( clicked() ), this, SLOT( OnCreate() ) );
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
    const bool authoring = config_.HasFeature( FEATURE_AUTHORING );
    const bool generation = config_.HasFeature( FEATURE_TERRAIN_GENERATION );
    progressPage_->SetTitle( tools::translate( "AuthoringPage", "Starting Application" ) );
    SetTextAndSubtitle( authoring_, tools::translate( "AuthoringPage", "Authoring" ),
        authoring
        ? tools::translate( "AuthoringPage", "Launch Authoring application" )
        : tools::translate( "AuthoringPage", "Missing Sword-Authoring license" ),
        !authoring );
    SetTextAndSubtitle( terrainGen_, tools::translate( "AuthoringPage", "Terrain Gen" ),
        generation
        ? tools::translate( "AuthoringPage", "Launch Terrain Generation application" )
        : tools::translate( "AuthoringPage", "Missing Sword-Terrain-Generation license" ),
        !generation );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::OnAuthoring
// Created: JSR 2010-06-10
// -----------------------------------------------------------------------------
void AuthoringPage::OnAuthoring()
{
    if( !config_.HasFeature( FEATURE_AUTHORING ) )
    {
        config_.CheckFeature( FEATURE_AUTHORING );
        OnLanguageChanged();
        return;
    }

    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::StartAuthoring >( config_ ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: AuthoringPage::OnCreate
// Created: BAX 2012-11-13
// -----------------------------------------------------------------------------
void AuthoringPage::OnCreate()
{
    if( !config_.HasFeature( FEATURE_TERRAIN_GENERATION ) )
    {
        config_.CheckFeature( FEATURE_TERRAIN_GENERATION );
        OnLanguageChanged();
        return;
    }
    create_->show();
}