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
#include "UserProfileUnitControls.h"
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
    : QTabWidget( parent, "UserProfileWidget" )
    , controllers_( controllers )
    , extensions_ ( extensions )
    , checker_    ( checker )
    , model_      ( model )
    , profile_    ( 0 )
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
        userRoleGroup_->setMargin( 5 );
        Q3HBox* roleBox = new Q3HBox( userRoleGroup_ );
        userRoleLabel_ = new QLabel( roleBox );
        userRole_ = new QComboBox( roleBox );
        userRoleLabel_->setText( tr( "Role" ));
        roleBox->setStretchFactor( userRoleLabel_, 1 );
        roleBox->setStretchFactor( userRole_, 1 );
        connect( userRole_, SIGNAL( activated( const QString& ) ), this, SLOT( OnUserRole( const QString& ) ) );
        userRoleGroup_->hide();
        addTab( box, tr( "General" ) );
        connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );
    }
    {
        Q3VBox* box = new Q3VBox( this );
        group_ = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
        group_->setMargin( 5 );

        Q3VBox* pFilterBox = new Q3VBox( group_ );

        pHidefilter_ = new QCheckBox( tr( "Hide automats controlled by another profile" ), pFilterBox );
        connect( pHidefilter_, SIGNAL( stateChanged( int ) ), this, SLOT( OnHideFilterChanged( int ) ) );
        pShowFilter_ = new QCheckBox( tr( "Show only automats controlled by this profile" ), pFilterBox );
        connect( pShowFilter_, SIGNAL( stateChanged( int ) ), this, SLOT( OnShowFilterChanged( int ) ) );

        QTabWidget* tabs = new QTabWidget( group_ );

        UserProfileUnitControls* unitRights = new UserProfileUnitControls( tabs, controllers, factory, icons, checker_, model );
        tabs->addTab( unitRights, tr( "Units" ) );
        pUnits_ = unitRights;

        UserProfilePopulationControls* populationRights = new UserProfilePopulationControls( tabs, controllers, factory, checker_ );
        tabs->addTab( populationRights, tr( "Crowds" ) );
        pPopulations_ = populationRights;

        addTab( box, tr( "Permissions" ) );

        QWidget* information = new QWidget( group_ );
        QVBoxLayout* layout = new QVBoxLayout( information );
        colorInformation_ = new QLabel( "<font color='#FF0A0A'>" + tr( "Units controlled by another profile." ) + "</font>", information );
        informationControls_ = new QLabel( tr( "'Control' permission allows you to control a unit." ), information );
        layout->addWidget( colorInformation_ );
        layout->addWidget( informationControls_ );
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
                    userRoleDico_->GetStringList( supervisors_, dicoKind_, dicoLanguage_, "readOnly" );
                    userRoleDico_->GetStringList( magicActions_, dicoKind_, dicoLanguage_, "magicActions" );
                    userRoleDico_->GetStringList( noEditable_, dicoKind_, dicoLanguage_, "noEditable" );
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
    if( userRoleDico_ )
    {
        const std::string role = profile_->GetUserRole();
        Update();
        if( !role.empty() )
            userRole_->setCurrentText( userRoleDico_->GetLabel( role, dicoKind_, dicoLanguage_ ).c_str() );
        else if( userRole_->currentText() != "" )
            profile_->SetUserRole( userRoleDico_->GetKey( userRole_->currentText().ascii(), dicoKind_, dicoLanguage_ ) );
    }

    pUnits_->Display( profile );
    pPopulations_->Display( profile );
    OnHideFilterChanged( pHidefilter_->checkState() );
    OnShowFilterChanged( pShowFilter_->checkState() );
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
// Name: UserProfileWidget::OnUserRole
// Created: JSR 2010-10-06
// -----------------------------------------------------------------------------
void UserProfileWidget::OnUserRole( const QString& role )
{
    if( userRoleDico_ && profile_ )
    {
        const std::string& key = userRoleDico_->GetKey( role.ascii(), dicoKind_, dicoLanguage_ );
        profile_->SetUserRole( key );
        pUnits_->Clear();
        Update();
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
// Name: UserProfileWidget::Update
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileWidget::Update()
{
    if( profile_ )
    {
        bool editable = std::find( noEditable_.begin(), noEditable_.end(), profile_->GetUserRole() ) == noEditable_.end();
        group_->setVisible( editable );
        if( !editable )
            profile_->Clear();
        bool supervisor = std::find( supervisors_.begin(), supervisors_.end(), profile_->GetUserRole() ) != supervisors_.end();
        informationControls_->setText( supervisor ? tr( "'View' permission allows you to see a unit." )
                                                  : tr( "'Control' permission allows you to control a unit." ) );
        colorInformation_->setVisible( !supervisor );
        pUnits_->Update( supervisor, *profile_ );
        pPopulations_->Update( supervisor, *profile_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Show
// Created: LGY 2011-09-28
// -----------------------------------------------------------------------------
void UserProfileWidget::Show()
{
    OnHideFilterChanged( pHidefilter_->checkState() );
    OnShowFilterChanged( pShowFilter_->checkState() );
    dynamic_cast< UserProfileUnitControls* >( pUnits_ )->Show();
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::Hide
// Created: LGY 2011-12-15
// -----------------------------------------------------------------------------
void UserProfileWidget::Hide()
{
    pUnits_->Clear();
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnHideFilterChanged
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileWidget::OnHideFilterChanged( int state )
{
    if( state == Qt::Checked )
        pUnits_->HideAssignedAutomats();
    else
    {
        pUnits_->RemoveFilter();
        if( pShowFilter_->isChecked() )
            pUnits_->ShowAssignedAutomats();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnShowFilterChanged
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileWidget::OnShowFilterChanged( int state )
{
    if( state == Qt::Checked )
        pUnits_->ShowAssignedAutomats();
    else
    {
        pUnits_->RemoveFilter();
        if( pHidefilter_->isChecked() )
            pUnits_->HideAssignedAutomats();
    }
}
