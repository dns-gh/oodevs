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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryType.h"
#include "clients_kernel/DictionaryEntryType.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Tools.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "preparation/Model.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/RichGroupBox.h"

using namespace kernel;

namespace
{
    QHBoxLayout* CreateLinelayout( const QString& text, gui::RichLineEdit* lineEdit )
    {
        QHBoxLayout* layout = new QHBoxLayout();
        layout->addWidget( new QLabel( text ) );
        layout->addWidget( lineEdit );
        return layout;
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( const QString& objectName, QWidget* parent, Controllers& controllers, const gui::EntitySymbols& icons,
                                      const ExtensionTypes& extensions, ProfilesChecker_ABC& checker, Model& model )
    : gui::RichWidget< QTabWidget >( objectName, parent )
    , controllers_( controllers )
    , extensions_ ( extensions )
    , checker_    ( checker )
    , model_      ( model )
    , profile_    ( 0 )
{
    gui::SubObjectName subObject( "UserProfileWidget" );
    {
        gui::SubObjectName subObject( "ProfileInformation" );
        //profile
        QLabel* loginLabel = new QLabel( tr( "Login:" ) );
        login_ = new gui::RichLineEdit( "login" );
        connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );

        //password
        QLabel* passwordLabel = new QLabel( tr( "Password:" ) );
        password_ = new gui::RichLineEdit( "password" );
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );

        //automat
        QLabel* automataLabel = new QLabel( tr( "Automats:" ) );
        automats_= new gui::RichLineEdit( "automats" );
        automats_->setReadOnly( true );

        //knowledge group
        QLabel* knowledgeLabel = new QLabel( tr( "Knowledge groups:" ) );
        knowledgeGroups_ = new gui::RichLineEdit( "knowledgeGroups" );
        knowledgeGroups_->setReadOnly( true );

        gui::RichGroupBox* profileInformationGroup = new gui::RichGroupBox( "ProfileInformation", tr( "Profile information" ) );
        QGridLayout* profileInformationGroupLayout = new QGridLayout( profileInformationGroup );
        profileInformationGroupLayout->addWidget( loginLabel, 0, 0 );
        profileInformationGroupLayout->addWidget( login_, 0, 1 );
        profileInformationGroupLayout->addWidget( passwordLabel, 1, 0 );
        profileInformationGroupLayout->addWidget( password_, 1, 1 );
        profileInformationGroupLayout->addWidget( automataLabel, 2, 0 );
        profileInformationGroupLayout->addWidget( automats_, 2, 1 );
        profileInformationGroupLayout->addWidget( knowledgeLabel, 3, 0 );
        profileInformationGroupLayout->addWidget( knowledgeGroups_, 3, 1 );
        profileInformationGroupLayout->setMargin( 5 );
        profileInformationGroupLayout->setColStretch( 0, 1 );
        profileInformationGroupLayout->setColStretch( 1, 5 );
        profileInformationGroupLayout->setSpacing( 0 );

        QWidget* box = new QWidget();
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->addWidget( profileInformationGroup );
        boxLayout->addStretch( 1 );
        addTab( box, tr( "General" ) );
    }
    {
        Q3VBox* box = new Q3VBox( this );

        Q3HBox* holder = new Q3HBox();
        supervisor_ = new gui::RichCheckBox( "supervisorActions", tr( "Supervisor actions" ), holder );
        gui::RichWidget< QTabWidget >* tabs = new gui::RichWidget< QTabWidget >( "RichWidget< QTabWidget >" );

        UserProfileUnitRights* unitRights = new UserProfileUnitRights( "unitRights", tabs, controllers, icons, tr( "Units" ) );
        tabs->addTab( unitRights, tr( "Units" ) );
        unitRights_ = unitRights;

        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( "populationRights", tabs, controllers, tr( "Crowds" ) );
        tabs->addTab( populationRights, tr( "Crowds" ) );
        populationRights_ = populationRights;

        addTab( box, tr( "Permissions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );
        QLabel* readPermissionlabel = new QLabel( tr( "'Read' permission allows you to see a unit.\n"
                        "'Write' permission allows you to control a unit." ) );

        gui::RichGroupBox* permissionsGroup = new gui::RichGroupBox( "permissionsGroup", tr( "Access permissions" ), box );
        QVBoxLayout* permissionsGroupLayout = new QVBoxLayout( permissionsGroup );
        permissionsGroupLayout->addWidget( holder );
        permissionsGroupLayout->addWidget( tabs );
        permissionsGroupLayout->addWidget( readPermissionlabel );
        permissionsGroupLayout->setMargin( 5 );
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

namespace
{
    const KnowledgeGroup_ABC* GetKnowledgeGroup( const Entity_ABC& entity )
    {
        const CommunicationHierarchies* commParentHirearchies = entity.Retrieve< CommunicationHierarchies >();
        if( !commParentHirearchies )
            return 0;
        const Entity_ABC* pSuperior = commParentHirearchies->GetSuperior();
        if( !pSuperior )
            return 0;
        const KnowledgeGroup_ABC* kg = dynamic_cast< const KnowledgeGroup_ABC* >( pSuperior );
        if( kg )
            return kg;
        return GetKnowledgeGroup( *pSuperior );
    }

    unsigned int GetKGCount( std::set< unsigned long > automats, Model& model )
    {
        std::set< unsigned long > knowledgeGroups;
        for( std::set< unsigned long >::iterator it = automats.begin(); it != automats.end(); ++it )
        {
            Automat_ABC* pAutomat = model.FindAutomat( *it );
            if( pAutomat )
            {
                const KnowledgeGroup_ABC* kg = GetKnowledgeGroup( *pAutomat );
                if( kg )
                    knowledgeGroups.insert( kg->GetId() );
            }
        }
        return static_cast< unsigned int >( knowledgeGroups.size() );
    }
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
    std::set< unsigned long > automats;
    profile.VisitAllAutomats( automats );
    automats_->setText( locale().toString( automats.size() ) );
    knowledgeGroups_->setText( locale().toString( GetKGCount( automats, model_ ) ) );
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
                throw MASA_EXCEPTION( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).toStdString().c_str() );
            if( profile_->GetLogin() != login && model_.profiles_.Exists( login ) && !checker_.Exists( login ) )
                throw MASA_EXCEPTION( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).toStdString().c_str() );
            profile_->SetLogin( login );
        }
    }
    catch( const std::exception& e )
    {
        QMessageBox::warning( this, tr( "Invalid profile information" ), tools::GetExceptionMsg( e ).c_str(), QMessageBox::Ok, Qt::NoButton );
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
    setVisible( enabled );
}
