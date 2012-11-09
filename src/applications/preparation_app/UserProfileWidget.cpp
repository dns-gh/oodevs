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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileWidget constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileWidget::UserProfileWidget( QWidget* parent, Controllers& controllers, const gui::EntitySymbols& icons,
                                      const ExtensionTypes& extensions, ProfilesChecker_ABC& checker, Model& model )
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
        new QLabel( tr( "Automats:" ), group );
        automats_= new QLineEdit( group );
        automats_->setReadOnly( true );
        new QLabel( tr( "Knowledge groups:" ), group );
        knowledgeGroups_ = new QLineEdit( group );
        knowledgeGroups_->setReadOnly( true );

        addTab( box, tr( "General" ) );
        connect( login_, SIGNAL( editingFinished() ), this, SLOT( OnLoginChanged() ) );
        connect( password_, SIGNAL( textChanged( const QString& ) ), SLOT( OnPasswordChanged( const QString& ) ) );
    }
    {
        Q3VBox* box = new Q3VBox( this );
        Q3GroupBox* group = new Q3GroupBox( 3, Qt::Vertical, tr( "Access permissions" ), box );
        group->setMargin( 5 );
        Q3HBox* holder = new Q3HBox( group );
        supervisor_ = new QCheckBox( tr( "Supervisor actions" ), holder );
        QTabWidget* tabs = new QTabWidget( group );

        UserProfileUnitRights* unitRights = new UserProfileUnitRights( tabs, controllers, icons, tr( "Units" ) );
        tabs->addTab( unitRights, tr( "Units" ) );
        unitRights_ = unitRights;

        UserProfilePopulationRights* populationRights = new UserProfilePopulationRights( tabs, controllers, tr( "Crowds" ) );
        tabs->addTab( populationRights, tr( "Crowds" ) );
        populationRights_ = populationRights;

        addTab( box, tr( "Permissions" ) );
        connect( supervisor_, SIGNAL( toggled( bool ) ), SLOT( OnSupervisorChanged( bool ) ) );
        new QLabel( tr( "'Read' permission allows you to see a unit.\n"
                        "'Write' permission allows you to control a unit." ), group );
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
                throw std::exception( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).toStdString().c_str() );
            if( profile_->GetLogin() != login && model_.profiles_.Exists( login ) && !checker_.Exists( login ) )
                throw std::exception( tools::translate( "UserProfileWidget", "Duplicate login: '%1'." ).arg( login ).toStdString().c_str() );
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
    setVisible( enabled );
}
