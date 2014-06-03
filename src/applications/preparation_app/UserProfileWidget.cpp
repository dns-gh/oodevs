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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Tools.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "preparation/Model.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( const QString& objectName, QWidget* parent, kernel::Controllers& controllers,
                                      const gui::EntitySymbols& icons, Model& model )
    : gui::RichWidget< QTabWidget >( objectName, parent )
    , controllers_( controllers )
    , model_      ( model )
    , profile_    ( 0 )
{
    gui::SubObjectName subObject( "UserProfileWidget" );
    {
        gui::SubObjectName subObject( "ProfileInformation" );
        //profile
        QLabel* loginLabel = new QLabel( tr( "Login:" ) );
        login_ = new gui::RichLineEdit( "login" );
        connect( login_, SIGNAL( editingFinished() ), SLOT( OnLoginChanged() ) );

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

        gui::RichGroupBox* group = new gui::RichGroupBox( "ProfileInformation", tr( "Profile information" ) );
        QGridLayout* layout = new QGridLayout( group );
        layout->addWidget( loginLabel, 0, 0 );
        layout->addWidget( login_, 0, 1 );
        layout->addWidget( passwordLabel, 1, 0 );
        layout->addWidget( password_, 1, 1 );
        layout->addWidget( automataLabel, 2, 0 );
        layout->addWidget( automats_, 2, 1 );
        layout->addWidget( knowledgeLabel, 3, 0 );
        layout->addWidget( knowledgeGroups_, 3, 1 );
        layout->setMargin( 5 );
        layout->setColStretch( 0, 1 );
        layout->setColStretch( 1, 5 );
        layout->setSpacing( 0 );

        QWidget* box = new QWidget();
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->addWidget( group );
        boxLayout->addStretch( 1 );
        addTab( box, tr( "General" ) );
    }
    {
        supervisor_ = new gui::RichCheckBox( "supervisorActions", tr( "Supervisor actions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );

        timeControl_  = new gui::RichCheckBox( "timeControl", tr( "Time Control" ) );
        connect( timeControl_, SIGNAL( toggled( bool ) ), SLOT( OnTimeControlChanged( bool ) ) );

        gui::RichWidget< QTabWidget >* tabs = new gui::RichWidget< QTabWidget >( "RichWidget< QTabWidget >" );

        unitRights_ = new UserProfileUnitRights( "unitRights", tabs, controllers, icons, tr( "Units" ) );
        tabs->addTab( unitRights_->GetWidget(), tr( "Units" ) );

        populationRights_ = new UserProfilePopulationRights( "populationRights", tabs, controllers, tr( "Crowds" ) );
        tabs->addTab( populationRights_->GetWidget(), tr( "Crowds" ) );

        QLabel* readPermissionlabel = new QLabel( tr( "'Read' permission allows you to see a unit.\n"
                        "'Write' permission allows you to control a unit." ) );

        gui::RichGroupBox* group = new gui::RichGroupBox( "permissionsGroup", tr( "Access permissions" ) );
        QHBoxLayout* holder = new QHBoxLayout();
        holder->addWidget( supervisor_ );
        holder->addWidget( timeControl_ );
        QVBoxLayout* layout = new QVBoxLayout( group );
        layout->addLayout( holder );
        layout->addWidget( tabs );
        layout->addWidget( readPermissionlabel );
        layout->setMargin( 5 );

        QWidget* box = new QWidget();
        QVBoxLayout* boxLayout = new QVBoxLayout( box );
        boxLayout->addWidget( group );
        boxLayout->addStretch( 1 );
        addTab( box, tr( "Permissions" ) );
    }
    setVisible( false );
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
    const kernel::KnowledgeGroup_ABC* GetKnowledgeGroup( const kernel::Entity_ABC& entity )
    {
        const kernel::CommunicationHierarchies* hierarchies = entity.Retrieve< kernel::CommunicationHierarchies >();
        if( !hierarchies )
            return 0;
        const kernel::Entity_ABC* pSuperior = hierarchies->GetSuperior();
        if( !pSuperior )
            return 0;
        const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( pSuperior );
        if( kg )
            return kg;
        return GetKnowledgeGroup( *pSuperior );
    }

    unsigned int GetKGCount( const std::set< unsigned long >& automats, Model& model )
    {
        std::set< unsigned long > knowledgeGroups;
        for( auto it = automats.begin(); it != automats.end(); ++it )
        {
            kernel::Automat_ABC* pAutomat = model.FindAutomat( *it );
            if( pAutomat )
            {
                const kernel::KnowledgeGroup_ABC* kg = GetKnowledgeGroup( *pAutomat );
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
    timeControl_->setChecked( profile.HasTimeControl() );
    unitRights_->Display( profile );
    populationRights_->Display( profile );
    setVisible( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::SetChecker
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
void UserProfileWidget::SetChecker( const ProfilesChecker_ABC* pChecker )
{
    pChecker_ = pChecker;
}

// -----------------------------------------------------------------------------
// Name: UserProfileWidget::OnLoginChanged
// Created: LGY 2011-09-26
// -----------------------------------------------------------------------------
void UserProfileWidget::OnLoginChanged()
{
    if( !profile_ || !pChecker_ )
        return;
    QString newLogin = login_->text();
    const QString& oldLogin = profile_->GetLogin();
    if( oldLogin == newLogin )
        return;
    if( pChecker_->Exists( oldLogin, newLogin ) )
    {
        QMessageBox::warning( this, tr( "Invalid profile information" ), tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( newLogin ), QMessageBox::Ok, Qt::NoButton );
        login_->setText( oldLogin );
    }
    else
    {
        profile_->SetLogin( newLogin );
        pChecker_->NotifyNameChanged( profile_ );
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
// Name: UserProfileWidget::OnTimeControlChanged
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
void UserProfileWidget::OnTimeControlChanged( bool timeControl )
{
    if( profile_ )
        profile_->SetTimeControl( timeControl );
}
