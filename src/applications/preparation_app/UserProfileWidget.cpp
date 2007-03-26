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
#include "UserProfileUnitRights.h"
#include "UserProfilePopulationRights.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons  )
    : QTabWidget( parent )
    , selectedProfile_( 0 )
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
    UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, factory, icons );
    tabs->addTab( unitRights, tr( "Units" ) );
    unitRights_ = unitRights;
    UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, factory );
    tabs->addTab( populationRights, tr( "Populations" ) );
    populationRights_ = populationRights;

    addTab( box, tr( "Rights" ) );
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
void UserProfileWidget::Display( const UserProfile& profile )
{
    login_->setText( profile.GetLogin() );
    password_->setText( profile.GetPassword() );
    supervisor_->setChecked( profile.IsSupervisor() );
    unitRights_->Display( profile );
    populationRights_->Display( profile );
    selectedProfile_ = &profile;
    SetEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Commit
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Commit( const ProfilesModel& model )
{
    if( CheckValidity( model ) )
    {
        UserProfile& profile = const_cast< UserProfile& >( *selectedProfile_ );
        profile.SetLogin( login_->text() );
        profile.SetPassword( password_->text() );
        profile.SetSupervisor( supervisor_->isChecked() );
        if( unitRights_->NeedSaving() )
            unitRights_->CommitTo( profile );
        if( populationRights_->NeedSaving() )
            populationRights_->CommitTo( profile );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Reset
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Reset()
{
    // $$$$ SBO 2007-01-17: TODO: somehow undo changes
    unitRights_->Reset();
    populationRights_->Reset();
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::CheckValidity
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
bool UserProfileWidget::CheckValidity( const ProfilesModel& model ) const
{
    if( !selectedProfile_ )
        return true;
    if( login_->text() != selectedProfile_->GetLogin() && model.Exists( login_->text() ) )
    {
        UserProfileWidget* that = const_cast< UserProfileWidget* >( this );
        QMessageBox::warning( that, tr( "Invalid profile information" )
                                  , tr( "Duplicate login: '%1'." ).arg( login_->text() )
                                  , QMessageBox::Ok, QMessageBox::NoButton );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::SetEnabled
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void UserProfileWidget::SetEnabled( bool enabled )
{
    setShown( enabled );
}
