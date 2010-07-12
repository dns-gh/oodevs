// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ProfilesPage.h"
#include "moc_ProfilesPage.cpp"
#include "Config.h"
#include "HomePage.h"
#include "MenuPage.h"
#include "OptionsPage.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ProfilesPage constructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
ProfilesPage::ProfilesPage( QWidgetStack* pages, Config& config, kernel::Controllers& controllers, NetworkExerciseLister& lister )
    : MenuPage( pages, *this, eButtonOptions | eButtonQuit )
    , config_( config )
    , homePage_( new HomePage( pages, *this, config, controllers, lister ) )
    , optionsPage_( new OptionsPage( pages, *this, config ) )
{
    AddLink( tools::translate( "ProfilesPage", "Terrain" ), *this, tools::translate( "ProfilesPage", "Select Terrain as profile" ), SLOT( OnTerrain() ) );
    AddLink( tools::translate( "ProfilesPage", "User" ), *this, tools::translate( "ProfilesPage", "Select User as profile" ), SLOT( OnUser() ) );
    AddLink( tools::translate( "ProfilesPage", "Advanced User" ), *this, tools::translate( "ProfilesPage", "Select Advanced User as profile" ), SLOT( OnAdvancedUser() ) );
    AddLink( tools::translate( "ProfilesPage", "Administrator" ), *this, tools::translate( "ProfilesPage", "Select Administrator as profile" ), SLOT( OnAdministrator() ) );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage destructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
ProfilesPage::~ProfilesPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage::OnTerrain
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void ProfilesPage::OnTerrain()
{
    config_.SetProfile( Config::eTerrain );
    homePage_->show();
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage::OnUser
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void ProfilesPage::OnUser()
{
    config_.SetProfile( Config::eUser );
    homePage_->show();
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage::OnAdvancedUser
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void ProfilesPage::OnAdvancedUser()
{
    config_.SetProfile( Config::eAdvancedUser );
    homePage_->show();
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage::OnAdministrator
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void ProfilesPage::OnAdministrator()
{
    config_.SetProfile( Config::eAdministrator );
    homePage_->show();
}

// -----------------------------------------------------------------------------
// Name: ProfilesPage::OnOptions
// Created: JSR 2010-06-04
// -----------------------------------------------------------------------------
void ProfilesPage::OnOptions()
{
    optionsPage_->show();
}
