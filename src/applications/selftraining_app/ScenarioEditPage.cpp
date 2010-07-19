// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioEditPage.h"
#include "moc_ScenarioEditPage.cpp"
#include "CreateExerciceWidget.h"
#include "ExerciseList.h"
#include "ExportWidget.h"
#include "ImportWidget.h"
#include "ProgressPage.h"
#include "ProcessWrapper.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/commands.h"
#include "frontend/EditExercise.h"
#include "frontend/ImportExercise.h"
#include "tools/GeneralConfig.h"
#include <qtabbar.h>
#include <qtabwidget.h>

namespace
{
    class TabWidget : public QTabWidget
    {
    public:
        TabWidget( QWidget* parent )
            : QTabWidget( parent )
        {
            setBackgroundOrigin( QWidget::WindowOrigin );
            tabBar()->setBackgroundOrigin( QWidget::WindowOrigin );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers )
    : ContentPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous, eButtonBack | eButtonEdit )
    , config_( config )
    , controllers_( controllers )
    , lister_( config_, "" )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioEditPage", "Editing exercise" ), controllers ) )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        mainTabs_ = new TabWidget( box );
        connect( mainTabs_, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( UpdateEditButton( QWidget* ) ) );
        {
            exercises_ = new ExerciseList( mainTabs_, config_, lister_, "", true, false );
            connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), this, SLOT( OnSelect( const QString&, const Profile& ) ) );
            mainTabs_->addTab( exercises_, tools::translate( "ScenarioEditPage", "Edit" ) );
        }
        {
            createExerciceWidget_ = new CreateExerciceWidget( *this, box, config );
            mainTabs_->addTab( createExerciceWidget_, tools::translate( "ScenarioEditPage", "Create" ) );
        }
        {
            importWidget_ = new ImportWidget( *this, mainTabs_, config );
            mainTabs_->addTab( importWidget_, tools::translate( "ScenarioEditPage", "Import" ) );
        }
        {
            exportWidget_ = new ExportWidget( *this, box, config );
            mainTabs_->addTab( exportWidget_, tools::translate( "ScenarioEditPage", "Export" ) );
        }
    }
    EnableButton( eButtonEdit, false );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage destructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::~ScenarioEditPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ExerciceExists
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool ScenarioEditPage::ExerciceExists( const QString& string )
{
    return exercises_->Exists( string );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::Update
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::Update()
{
    createExerciceWidget_->Update();
    exercises_->Update();
    exportWidget_->Update();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditExercise
// Created: SBO 2008-10-07
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnEdit()
{
    switch( mainTabs_->currentPageIndex() )
    {
    case 0: //edit
        if( !exercise_.isEmpty() )
        {
            exercises_->ChangeExerciceParameters( exercise_.ascii() );
            Edit( exercise_ );
        }
        break;
    case 1: // create
        createExerciceWidget_->CreateExercise();
        break;
    case 2: // import
        importWidget_->InstallExercise();
        break;
    case 3: // export
        exportWidget_->ExportPackage();
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::Edit
// Created: SBO 2008-10-16
// -----------------------------------------------------------------------------
void ScenarioEditPage::Edit( const QString& exercise )
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, exercise, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::OnSelect
// Created: SBO 2010-04-15
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnSelect( const QString& exercise, const Profile& )
{
    exercise_ = exercise;
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditNameChanged
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void ScenarioEditPage::EditNameChanged( const QString& )
{
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ComboChanged
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void ScenarioEditPage::ComboChanged( int )
{
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::UpdateEditButton
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
void ScenarioEditPage::UpdateEditButton()
{
    bool enable = true;
    switch( mainTabs_->currentPageIndex() )
    {
    case 0: //edit
        enable = !exercise_.isEmpty();
        SetButtonText( eButtonEdit, tools::translate( "Page_ABC", "Edit" ) );
        break;
    case 1: // create
        enable = createExerciceWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Create" ) );
        break;
    case 2: // import
        enable = importWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Import" ) );
        break;
    case 3: // export
        enable = exportWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Export" ) );
        break;
    default:
        break;
    }
    EnableButton( eButtonEdit, enable );
}


// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::UpdateEditButton
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ScenarioEditPage::UpdateEditButton( QWidget* )
{
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::LaunchScenarioImport
// Created: LDC 2010-07-06
// LTO
// -----------------------------------------------------------------------------
void ScenarioEditPage::LaunchScenarioImport( const QString& inputScenario, const QString& outputScenario )
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::ImportExercise( config_, inputScenario, outputScenario, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();        
}
    
// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::LaunchPreparation
// Created: LDC 2010-07-06
// LTO
// -----------------------------------------------------------------------------
void ScenarioEditPage::LaunchPreparation( const QString& outputScenario )
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, outputScenario, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}
