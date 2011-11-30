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
#include "UserProfileUnitControls.h"
#include "UserProfilePopulationRights.h"
#include "UserProfilePopulationControls.h"
#include "ControlsChecker_ABC.h"
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
                                      ControlsChecker_ABC& checker, Model& model )
    : QTabWidget   ( parent, "UserProfileWidget" )
    , controllers_ ( controllers )
    , extensions_  ( extensions )
    , checker_     ( checker )
    , model_       ( model )
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
        connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );
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

        pUnits_ = new QStackedWidget( tabs );
        UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, factory, icons );
        pUnits_->addWidget( ( Q3ListView* ) unitRights );
        tabs->addTab( pUnits_, tr( "Units" ) );

        UserProfileUnitControls* unitControls = new UserProfileUnitControls( tabs, controllers, factory, icons, checker_, model );
        pUnits_->addWidget( ( Q3ListView* ) unitControls );
        connect( unitControls, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ), unitRights, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ) );
        connect( unitRights, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool ) ), unitControls, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool ) ) );

        pPopulations_ = new QStackedWidget( tabs );
        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, factory );
        pPopulations_->addWidget( ( Q3ListView* ) populationRights );
        tabs->addTab( pPopulations_, tr( "Crowds" ) );

        UserProfilePopulationControls* populationControls = new UserProfilePopulationControls( tabs, controllers, factory, checker_ );
        pPopulations_->addWidget( ( Q3ListView* ) populationControls );
        connect( populationControls, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ), populationRights, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool, bool ) ) );
        connect( populationRights, SIGNAL( ProfiledChanged( const kernel::Entity_ABC*, bool ) ), populationControls, SLOT( OnProfiledChanged( const kernel::Entity_ABC*, bool ) ) );

        addTab( box, tr( "Permissions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );

        pInformations_ = new QStackedWidget( group );
        QLabel* informationRights = new QLabel( tr( "'Read' permission allows you to see an unit.\n"
                                                    "'Write' permission allows you to control an unit." ), group );
        informationControls_ = new QLabel( tr( "'Control' permission allows you to control an unit." ), group );
        pInformations_->addWidget( informationRights );
        pInformations_->addWidget( informationControls_ );
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
                    userRoleDico_->GetStringList( supervisors_, dicoKind_, dicoLanguage_, "supervisor" );
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
    dynamic_cast< UserProfileRights_ABC* >( pUnits_->widget( 0 ) )->Display( profile );
    dynamic_cast< UserProfileControls_ABC* >( pUnits_->widget( 1 ) )->Display( profile );
    dynamic_cast< UserProfileRights_ABC* >( pPopulations_->widget( 0 ) )->Display( profile );
    dynamic_cast< UserProfileControls_ABC* >( pPopulations_->widget( 1 ) )->Display( profile );
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
    {
        profile_->SetUserRole( userRoleDico_->GetKey( role.ascii(), dicoKind_, dicoLanguage_ ) );
        ActivateControls();
    }
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
    pUnits_->setCurrentIndex( 1 );
    pInformations_->setCurrentIndex( 1 );
    if( profile_ )
    {
        bool supervisor = std::find( supervisors_.begin(), supervisors_.end(), profile_->GetUserRole() ) != supervisors_.end();
        informationControls_->setText( supervisor ? tr( "'View' permission allows you to see an unit." )
                                                  : tr( "'Control' permission allows you to control an unit." ) );
        dynamic_cast< UserProfileControls_ABC* >( pUnits_->widget( 1 ) )->Update( supervisor );
        dynamic_cast< UserProfileControls_ABC* >( pPopulations_->widget( 1 ) )->Update( supervisor );
        OnSupervisorChanged( supervisor );
    }
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
    pUnits_->setCurrentIndex( 0 );
    pInformations_->setCurrentIndex( 0 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Show
// Created: LGY 2011-09-28
// -----------------------------------------------------------------------------
void UserProfileWidget::Show()
{
    dynamic_cast< UserProfileUnitControls* >( pUnits_->widget( 1 ) )->Show();
}
