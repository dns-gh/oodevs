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
#include "UserProfilePopulationControls.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/DictionaryEntryType.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "preparation/UserProfile.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, const ExtensionTypes& extensions )
    : QTabWidget   ( parent, "UserProfileWidget" )
    , controllers_ ( controllers )
    , extensions_  ( extensions )
    , profile_     ( 0 )
    , userRoleDico_( 0 )
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
        userRoleGroup_ = new Q3GroupBox( 1, Qt::Horizontal,  tr( "User role" ), box );
        userRoleGroup_->setCheckable( true );
        userRoleGroup_->setMargin( 5 );
        Q3HBox* roleBox = new Q3HBox( userRoleGroup_ );
        userRoleLabel_ = new QLabel( roleBox );
        userRole_ = new QComboBox( roleBox );
        userRoleLabel_->setText( tr( "Role" ));
        roleBox->setStretchFactor( userRoleLabel_, 1 );
        roleBox->setStretchFactor( userRole_, 1 );
        connect( userRoleGroup_, SIGNAL( toggled( bool ) ), this, SLOT( OnUserRoleActivation( bool ) ) );
        connect( userRole_, SIGNAL( activated( const QString& ) ), this, SLOT( OnUserRole( const QString& ) ) );
        userRoleGroup_->hide();
        addTab( box, tr( "General" ) );
        connect( login_, SIGNAL( textChanged( const QString& ) ), SLOT( OnLoginChanged( const QString& ) ) );
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );
    }
    {
        Q3VBox* box = new Q3VBox( this );
        Q3GroupBox* group = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
        group->setMargin( 5 );
        Q3HBox* holder = new Q3HBox( group );
        supervisorLabel_ = new QLabel( tr( "Supervisor actions:" ), holder );
        supervisor_ = new QCheckBox( holder );
        QTabWidget* tabs = new QTabWidget( group );

        UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, factory, icons );
        tabs->addTab( unitRights, tr( "Units" ) );
        unitRights_ = unitRights;

        pPopulations_ = new QStackedWidget( tabs );
        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, factory );
        pPopulations_->addWidget( ( Q3ListView*) populationRights );
        tabs->addTab( pPopulations_, tr( "Crowds" ) );

        UserProfilePopulationControls* populationControls = new UserProfilePopulationControls( tabs, controllers, factory );
        pPopulations_->addWidget( ( Q3ListView*) populationControls );
        connect( populationControls, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ), populationRights, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ) );
        connect( populationRights, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool ) ), populationControls, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool ) ) );

        addTab( box, tr( "Permissions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );
        new QLabel( tr( "'Read' permission allows you to see an unit.\n"
                        "'Write' permission allows you to control an unit." ), group );
    }
    SetEnabled( false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::~UserProfileWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::NotifyUpdated
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfileWidget::NotifyUpdated( const kernel::ModelLoaded& )
{
    userRole_->clear();
    userRoleDico_ = 0;
    userRoleGroup_->hide();
    ExtensionType* type = extensions_.tools::StringResolver< ExtensionType >::Find( "profile-attributes" );
    if( type )
    {
        tools::Iterator< const AttributeType& > it = type->CreateIterator();
        while( it.HasMoreElements() )
        {
            const AttributeType& attribute = it.NextElement();
            if( attribute.GetType() == AttributeType::ETypeDictionary )
            {
                std::string dictionary;
                attribute.GetDictionaryValues( dictionary, dicoKind_, dicoLanguage_ );
                userRoleDico_ = extensions_.tools::StringResolver< DictionaryType >::Find( dictionary );
                if( userRoleDico_ )
                {
                    QStringList list;
                    userRoleDico_->GetStringList( list, dicoKind_, dicoLanguage_ );
                    userRole_->insertStringList( list );
                    userRoleGroup_->show();
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::NotifyUpdated
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfileWidget::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    userRoleDico_ = 0;
    userRole_->clear();
    userRoleGroup_->hide();
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
    if( userRoleDico_ )
    {
        const std::string role = profile_->GetUserRole();
        userRoleGroup_->setChecked( ! role.empty() );
        if( userRoleGroup_->isChecked() )
            ActivateControls();
        if( !role.empty() )
            userRole_->setCurrentText( userRoleDico_->GetLabel( role, dicoKind_, dicoLanguage_ ).c_str() );
    }
    unitRights_->Display( profile );
    dynamic_cast< UserProfileRights_ABC* >( pPopulations_->widget( 0 ) )->Display( profile );
    dynamic_cast< UserProfileControls_ABC* >( pPopulations_->widget( 1 ) )->Display( profile );
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
// Name: UserProfileWidget::OnUserRoleActivation
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfileWidget::OnUserRoleActivation( bool enable )
{
    if( userRoleDico_ && profile_ )
    {
        if( enable && profile_->GetUserRole().empty() )
        {
            profile_->SetUserRole( userRoleDico_->GetKey( userRole_->currentText().ascii(), dicoKind_, dicoLanguage_ ) );
            ActivateControls();
        }
        else if( !enable )
        {
            profile_->SetUserRole( "" );
            DeactivateControls();
        }
        controllers_.controller_.Update( profile_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnUserRole
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfileWidget::OnUserRole( const QString& role )
{
    if( userRoleDico_ && profile_ )
        profile_->SetUserRole( userRoleDico_->GetKey( role.ascii(), dicoKind_, dicoLanguage_ ) );
    controllers_.controller_.Update( profile_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::SetEnabled
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void UserProfileWidget::SetEnabled( bool enabled )
{
    setShown( enabled );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::ActivateControls
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileWidget::ActivateControls()
{
    supervisorLabel_->hide();
    supervisor_->hide();
    pPopulations_->setCurrentIndex( 1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::DeactivateControls
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileWidget::DeactivateControls()
{
    supervisorLabel_->show();
    supervisor_->show();
    pPopulations_->setCurrentIndex( 0 );
}
