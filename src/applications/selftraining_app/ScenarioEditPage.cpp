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
#include "MessageDialog.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/ImportExercise.h"
#include "frontend/ProcessWrapper.h"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( QWidget* parent, Q3WidgetStack* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonEdit | eButtonDelete, launcher )
    , parent_      ( parent )
    , config_      ( config )
    , fileLoader_  ( fileLoader )
    , controllers_ ( controllers )
    , progressPage_( new ProgressPage( pages, *this ) )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 5 );
    {
        mainTabs_ = new QTabWidget( box );
        connect( mainTabs_, SIGNAL( currentChanged( QWidget* ) ), this, SLOT( UpdateEditButton( QWidget* ) ) );
        // eTabs_Edit
        {
            exercises_ = new ExerciseList( mainTabs_, config_, fileLoader_, controllers, true, false );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
            mainTabs_->addTab( exercises_, "" );
        }
        // eTabs_Create
        {
            createExerciceWidget_ = new CreateExerciceWidget( *this, box, config, fileLoader );
            mainTabs_->addTab( createExerciceWidget_, "" );
        }
        // eTabs_Import
        {
            importWidget_ = new ImportWidget( *this, mainTabs_, config );
            mainTabs_->addTab( importWidget_, "" );
        }
        // eTabs_Export
        {
            exportWidget_ = new ExportWidget( *this, box, config, fileLoader_ );
            mainTabs_->addTab( exportWidget_, "" );
        }
    }
    EnableButton( eButtonDelete, false );
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
// Name: ScenarioEditPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnLanguageChanged()
{
    SetTitle( tools::translate( "ScenarioEditPage", "Scenario" ) );
    progressPage_->SetTitle(  tools::translate( "ScenarioEditPage", "Editing exercise" ) );
    mainTabs_->setTabText( eTabs_Edit, tools::translate( "ScenarioEditPage", "Edit" ) );
    mainTabs_->setTabText( eTabs_Create, tools::translate( "ScenarioEditPage", "Create" ) );
    mainTabs_->setTabText( eTabs_Import, tools::translate( "ScenarioEditPage", "Import" ) );
    mainTabs_->setTabText( eTabs_Export, tools::translate( "ScenarioEditPage", "Export" ) );
    UpdateEditButton();
    LauncherClientPage::OnLanguageChanged();
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
    EnableButton( eButtonEdit, false );
    EnableButton( eButtonDelete, false );
    switch( mainTabs_->currentPageIndex() )
    {
    case eTabs_Edit:
        if( exercise_ )
        {
            exercises_->ChangeExerciceParameters();
            Edit( exercise_->GetName().c_str() );
        }
        break;
    case eTabs_Create:
        createExerciceWidget_->CreateExercise();
        break;
    case eTabs_Import:
        importWidget_->InstallExercise();
        break;
    case eTabs_Export:
        exportWidget_->ExportPackage();
        break;
    default:
        break;
    }
    EnableButton( eButtonDelete, true );
    EnableButton( eButtonEdit, true );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::OnDelete
// Created: LGY 2012-01-19
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnDelete()
{
    if( exercise_ )
    {
        const std::string path = config_.GetExerciseDir( exercise_->GetName().c_str() );
        MessageDialog message( parent_, tools::translate( "ScenarioEditPage", "Delete exercise" ), tools::translate( "ScenarioEditPage", "Are you sure you want to delete this exercise?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() == QMessageBox::Yes )
        {
            if( bfs::exists( path ) && bfs::is_directory( path ) )
            {
                bfs::remove_all( path );
                Update();
            }
        }
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
    case eTabs_Edit:
        enable = exercise_ != 0;
        SetButtonText( eButtonDelete, tools::translate( "Page_ABC", "Delete" ) );
        SetButtonText( eButtonEdit, tools::translate( "Page_ABC", "Edit" ) );
        break;
    case eTabs_Create:
        enable = createExerciceWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Create" ) );
        break;
    case eTabs_Import:
        enable = importWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Import" ) );
        break;
    case eTabs_Export:
        enable = exportWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Export" ) );
        break;
    default:
        break;
    }
    EnableButton( eButtonDelete, enable );
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
