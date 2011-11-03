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
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/ImportExercise.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/TabWidget.h"

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( Q3WidgetStack* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous, eButtonBack | eButtonEdit, launcher )
    , config_( config )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioEditPage", "Editing exercise" ) ) )
{
    Q3VBox* box = new Q3VBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        mainTabs_ = new frontend::TabWidget( box );
        connect( mainTabs_, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( UpdateEditButton( QWidget* ) ) );
        {
            exercises_ = new ExerciseList( mainTabs_, config_, fileLoader_, controllers, true, false );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
            mainTabs_->addTab( exercises_, tools::translate( "ScenarioEditPage", "Edit" ) );
        }
        {
            createExerciceWidget_ = new CreateExerciceWidget( *this, box, config, fileLoader );
            mainTabs_->addTab( createExerciceWidget_, tools::translate( "ScenarioEditPage", "Create" ) );
        }
        {
            importWidget_ = new ImportWidget( *this, mainTabs_, config );
            mainTabs_->addTab( importWidget_, tools::translate( "ScenarioEditPage", "Import" ) );
        }
        {
            exportWidget_ = new ExportWidget( *this, box, config, fileLoader_ );
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
    exercises_->Clear();
    Connect( "localhost", config_.GetLauncherPort() );
    createExerciceWidget_->Update();
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
        if( exercise_ )
        {
            exercises_->ChangeExerciceParameters();
            Edit( exercise_->GetName().c_str() );
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
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::OnSelect
// Created: SBO 2010-04-15
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnSelect( const frontend::Exercise_ABC& exercise )
{
    exercise_ = &exercise;
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void ScenarioEditPage::ClearSelection()
{
    exercise_ = 0;
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
// Name: ScenarioEditPage::ToggleChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void ScenarioEditPage::ToggleChanged( bool )
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
        enable = exercise_ != 0;
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
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
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
    boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
    progressPage_->Attach( process );
    process->Start();
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ShowPackageInstallation
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void ScenarioEditPage::ShowPackageInstallation( const QString& package )
{
    importWidget_->SelectPackage( package );
    mainTabs_->showPage( importWidget_ );
    QTimer::singleShot( 1, this, SLOT( show() ) );
}
