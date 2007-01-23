// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileWidget.h"
#include "UserProfileUnitRights.h"
#include "UserProfilePopulationRights.h"
#include "gaming/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons )
    : QTabWidget( parent )
    , selectedProfile_( controllers )
{
    QVBox* box = new QVBox( this );
    box->setSpacing( 5 );
    QGroupBox* group = new QGroupBox( 2,  Qt::Horizontal, tr( "Profile information" ), box );
    group->setMargin( 5 );
    new QLabel( tr( "Login:" ), group );
    login_ = new QLineEdit( group ); 
    new QLabel( tr( "Password:" ), group );
    password_ = new QLineEdit( group );
    addTab( box, tr( "General" ) );

    box = new QVBox( this );
    group = new QGroupBox( 2, Qt::Vertical, tr( "Access rights" ), box );
    group->setMargin( 5 );
    QHBox* holder = new QHBox( group );
    new QLabel( tr( "Supervisor actions:" ), holder );
    supervisor_ = new QCheckBox( holder );
    QTabWidget* tabs = new QTabWidget( group );
    UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, itemFactory, profile, icons );
    tabs->addTab( unitRights, tr( "Units" ) );
    unitRights_ = unitRights;
    UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, itemFactory, profile );
    tabs->addTab( populationRights, tr( "Populations" ) );
    populationRights_ = populationRights;

    addTab( box, tr( "Rights" ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::~UserProfileWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Display
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Display( const UserProfile& profile )
{
    login_->setText( profile.GetLogin() );
    password_->setText( profile.GetPassword() );
    supervisor_->setChecked( profile.IsSupervisor() );
    unitRights_->Display( profile );
    populationRights_->Display( profile );
    selectedProfile_ = &profile;
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Commit
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Commit()
{
    if( selectedProfile_ )
    {
        std::auto_ptr< UserProfile > profile( &selectedProfile_->Clone() );
        profile->SetPassword  ( password_->text() );
        profile->SetSupervisor( supervisor_->isChecked() );
        unitRights_      ->CommitTo( *profile );
        populationRights_->CommitTo( *profile );
        profile->RequestUpdate( login_->text() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Reset
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Reset()
{
    if( selectedProfile_ )
        Display( *selectedProfile_ );
}
