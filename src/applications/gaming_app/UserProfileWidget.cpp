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
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
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
    if( selectedProfile_ && selectedProfile_ != &profile && NeedsSaving() )
        if( QMessageBox::question( this, tr( "Profile edition" ), tr( "Profile has changed, commit modifications?" )
                                 , QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            Commit();

    editedProfile_.reset( new UserProfile( profile ) );
    login_->setText( editedProfile_->GetLogin() );
    password_->setText( editedProfile_->GetPassword() );
    supervisor_->setChecked( editedProfile_->IsSupervisor() );
    unitRights_->Display( *editedProfile_ );
    populationRights_->Display( *editedProfile_ );
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
        editedProfile_->SetPassword  ( password_->text() );
        editedProfile_->SetSupervisor( supervisor_->isChecked() );
        if( unitRights_->NeedsSaving() )
            unitRights_->Commit( true );
        if( populationRights_->NeedsSaving() )
            populationRights_->Commit( true );
        editedProfile_->RequestUpdate( login_->text() );
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

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::NeedsSaving
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
bool UserProfileWidget::NeedsSaving() const
{
    if( !selectedProfile_ )
        return false;
    return selectedProfile_->GetLogin() != login_->text() || selectedProfile_->GetPassword() != password_->text()
        || selectedProfile_->IsSupervisor() != supervisor_->isChecked()
        || unitRights_->NeedsSaving() || populationRights_->NeedsSaving();
}
