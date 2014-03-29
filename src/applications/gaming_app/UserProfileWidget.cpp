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
#include "moc_UserProfileWidget.cpp"
#include "UserProfileUnitRights.h"
#include "UserProfilePopulationRights.h"
#include "gaming/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : QTabWidget( parent, "UserProfileWidget" )
    , selectedProfile_( controllers )
    , profile_( profile )
{
    Q3VBox* box = new Q3VBox( this );
    box->setSpacing( 5 );
    Q3GroupBox* group = new Q3GroupBox( 2,  Qt::Horizontal, tr( "Profile information" ), box );
    group->setMargin( 5 );
    new QLabel( tr( "Login:" ), group );
    login_ = new QLineEdit( group );
    new QLabel( tr( "Password:" ), group );
    password_ = new QLineEdit( group );
    addTab( box, tr( "General" ) );

    box = new Q3VBox( this );
    group = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
    group->setMargin( 5 );
    Q3HBox* holder = new Q3HBox( group );
    supervisor_ = new QCheckBox( tr( "Supervisor actions" ), holder );
    timeControl_ = new QCheckBox( tr( "Time Control" ), holder );
    QTabWidget* tabs = new QTabWidget( group );
    UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, icons, tr( "Units" ), profile );
    tabs->addTab( unitRights, tr( "Units" ) );
    unitRights_ = unitRights;
    UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, tr( "Crowds" ), profile );
    tabs->addTab( populationRights, tr( "Crowds" ) );
    populationRights_ = populationRights;
    addTab( box, tr( "Permissions" ) );
    new QLabel( tr( "'Read' permission allows you to see a unit.\n"
                    "'Write' permission allows you to control a unit." ), group );
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
    // $$$$ AGE 2007-08-27: pas de QMessageBox statique :
    if( selectedProfile_ && selectedProfile_ != &profile && NeedsSaving() )
        if( QMessageBox::question( this, tr( "Profile edition" ), tr( "Profile has changed, commit modifications?" )
                                 , QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
            Commit();
    setEnabled( true );
    editedProfile_.reset( new UserProfile( profile ) );
    login_->setText( editedProfile_->GetLogin() );
    password_->setText( editedProfile_->GetPassword() );
    supervisor_->setChecked( editedProfile_->IsSupervision() );
    timeControl_->setChecked( editedProfile_->HasTimeControl() );
    timeControl_->setEnabled( profile_.HasTimeControl() );
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
    if( !selectedProfile_ )
        return;
    editedProfile_->SetPassword  ( password_->text() );
    editedProfile_->SetSupervisor( supervisor_->isChecked() );
    editedProfile_->SetTimeControl( timeControl_->isChecked() );
    if( unitRights_->NeedsSaving() )
        unitRights_->Commit( true );
    if( populationRights_->NeedsSaving() )
        populationRights_->Commit( true );
    editedProfile_->RequestUpdate( login_->text() );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Clean
// Created: LGY 2011-04-05
// -----------------------------------------------------------------------------
void UserProfileWidget::Clean()
{
    selectedProfile_ = 0;
    setDisabled( true );
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
    if( !isVisible() || !selectedProfile_ )
        return false;
    return selectedProfile_->GetLogin() != login_->text() || selectedProfile_->GetPassword() != password_->text()
        || selectedProfile_->IsSupervision() != supervisor_->isChecked()
        || selectedProfile_->HasTimeControl() != timeControl_->isChecked()
        || unitRights_->NeedsSaving() || populationRights_->NeedsSaving();
}
