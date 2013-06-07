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
#include "clients_kernel/Controller.h"
#include "gaming/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : QTabWidget( parent, "UserProfileWidget" )
    , controller_( controllers.controller_ )
    , editedProfile_( 0 )
{
    Q3VBox* box = new Q3VBox( this );
    box->setSpacing( 5 );
    Q3GroupBox* group = new Q3GroupBox( 2,  Qt::Horizontal, tr( "Profile information" ), box );
    group->setMargin( 5 );
    new QLabel( tr( "Login:" ), group );
    login_ = new QLineEdit( group );
    connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );
    new QLabel( tr( "Password:" ), group );
    password_ = new QLineEdit( group );
    connect( password_, SIGNAL( editingFinished() ), this, SLOT( OnPasswordChanged() ) );
    addTab( box, tr( "General" ) );

    box = new Q3VBox( this );
    group = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
    group->setMargin( 5 );
    Q3HBox* holder = new Q3HBox( group );
    new QLabel( tr( "Supervisor actions:" ), holder );
    supervisor_ = new QCheckBox( holder );
    connect( supervisor_, SIGNAL( stateChanged( int ) ), SLOT( OnSupervisionChanged( int ) ) );
    QTabWidget* tabs = new QTabWidget( group );
    UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, itemFactory, profile, icons );
    tabs->addTab( unitRights, tr( "Units" ) );
    unitRights_ = unitRights;
    UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, itemFactory, profile );
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
void UserProfileWidget::Display( UserProfile& profile )
{
    if( editedProfile_ )
    {
        if( unitRights_->NeedsCommit() )
            unitRights_->Commit( unitRights_->NeedsCommit() );
        if( populationRights_->NeedsCommit() )
            populationRights_->Commit( unitRights_->NeedsCommit() );
    }
    editedProfile_ = &profile;
    login_->setText( profile.GetLogin() );
    password_->setText( profile.GetPassword() );
    supervisor_->setChecked( profile.IsSupervisor() );
    unitRights_->Display( profile );
    populationRights_->Display( profile );
    setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Clean
// Created: LGY 2011-04-05
// -----------------------------------------------------------------------------
void UserProfileWidget::Clean()
{
    editedProfile_ = 0;
    unitRights_->Clear();
    populationRights_->Clear();
    setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Reset
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileWidget::Reset()
{
    if( editedProfile_ )
        Display( *editedProfile_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnLoginChanged
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void UserProfileWidget::OnLoginChanged()
{
    if( editedProfile_ )
        editedProfile_->SetLogin( login_->text() );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnPasswordChanged
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileWidget::OnPasswordChanged()
{
    if( editedProfile_ )
        editedProfile_->SetPassword( password_->text() );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnSupervisionChanged
// Created: NPT 2013-05-23
// -----------------------------------------------------------------------------
void UserProfileWidget::OnSupervisionChanged( int change )
{
    if( editedProfile_ )
        editedProfile_->SetSupervisor( change == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::NeedsSaving
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
bool UserProfileWidget::NeedsSaving() const
{
    if( !isVisible() || !editedProfile_ )
        return false;
    return editedProfile_->GetLogin() != login_->text() || editedProfile_->GetPassword() != password_->text()
        || editedProfile_->IsSupervisor() != supervisor_->isChecked()
        || unitRights_->NeedsSaving() || populationRights_->NeedsSaving();
}
