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
#include "ProfilesChecker_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/DictionaryEntryType.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "preparation/Model.h"
#include "clients_kernel/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory,
                                      gui::EntitySymbols& icons, const ExtensionTypes& extensions,
                                      ProfilesChecker_ABC& checker, Model& model )
    : QTabWidget( parent, "UserProfileWidget" )
    , controllers_( controllers )
    , extensions_ ( extensions )
    , checker_    ( checker )
    , model_      ( model )
    , profile_    ( 0 )
{
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
        connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );
    }
    {
        Q3VBox* box = new Q3VBox( this );
        Q3GroupBox* group = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
        group->setMargin( 5 );
        Q3HBox* holder = new Q3HBox( group );
        new QLabel( tr( "Supervisor actions:" ), holder );
        supervisor_ = new QCheckBox( holder );
        QTabWidget* tabs = new QTabWidget( group );

        UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, factory, icons );
        tabs->addTab( unitRights, tr( "Units" ) );
        unitRights_ = unitRights;

        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, factory );
        tabs->addTab( populationRights, tr( "Crowds" ) );
        populationRights_ = populationRights;

        addTab( box, tr( "Permissions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );
        new QLabel( tr( "'Read' permission allows you to see an unit.\n"
                        "'Write' permission allows you to control an unit." ), group );
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
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void UserProfileWidget::OnLoginChanged()
{
    try
    {
        if( profile_ )
        {
            QString login = login_->text();
            if( profile_->GetLogin() != login && checker_.Exists( profile_->GetLogin(), login ) )
                throw std::exception( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).ascii() );
            if( profile_->GetLogin() != login && model_.profiles_.Exists( login ) && !checker_.Exists( login ) )
                throw std::exception( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).ascii() );
            profile_->SetLogin( login );
        }
    }
    catch( std::exception& e )
    {
        QMessageBox::warning( this, tr( "Invalid profile information" ), e.what(), QMessageBox::Ok, Qt::NoButton );
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
