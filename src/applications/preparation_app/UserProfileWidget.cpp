// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileWidget.h"
#include "moc_UserProfileWidget.cpp"
#include "UserProfileUnitRights.h"
#include "UserProfilePopulationRights.h"
#include "preparation/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons )
    : QTabWidget( parent )
    , profile_( 0 )
{
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
        connect( login_   , SIGNAL( textChanged( const QString& ) ), SLOT( OnLoginChanged   ( const QString& ) ) ); 
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );
    }
    {
        QVBox* box = new QVBox( this );
        QGroupBox* group = new QGroupBox( 2, Qt::Vertical, tr( "Access rights" ), box );
        group->setMargin( 5 );
        QHBox* holder = new QHBox( group );
        new QLabel( tr( "Supervisor actions:" ), holder );
        supervisor_ = new QCheckBox( holder );
        QTabWidget* tabs = new QTabWidget( group );
        UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, factory, icons );
        tabs->addTab( unitRights, tr( "Units" ) );
        unitRights_ = unitRights;
        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, factory );
        tabs->addTab( populationRights, tr( "Populations" ) );
        populationRights_ = populationRights;
        addTab( box, tr( "Rights" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );
    }
    SetEnabled( false );
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
void UserProfileWidget::Display( UserProfile& profile )
{
    profile_ = &profile;
    login_->setText( profile.GetLogin() );
    password_->setText( profile.GetPassword() );
    supervisor_->setChecked( profile.IsSupervisor() );
    unitRights_->Display( profile );
    populationRights_->Display( profile );
    SetEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnLoginChanged
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileWidget::OnLoginChanged( const QString& text )
{
    try
    {
        if( profile_ )
            profile_->SetLogin( text );
    }
    catch( std::exception& e )
    {
        QMessageBox::warning( this, tr( "Invalid profile information" ), e.what(), QMessageBox::Ok, QMessageBox::NoButton );
        login_->setText( profile_->GetLogin() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnPasswordChanged
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileWidget::OnPasswordChanged( const QString& text )
{
    if( profile_ )
        profile_->SetPassword( text );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnSupervisorChanged
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileWidget::OnSupervisorChanged( bool supervisor )
{
    if( profile_ )
        profile_->SetSupervisor( supervisor );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::SetEnabled
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void UserProfileWidget::SetEnabled( bool enabled )
{
    setShown( enabled );
}
