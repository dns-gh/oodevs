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
#include "ProgressPage.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/Config.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/ImportExercise.h"
#include "frontend/ProcessWrapper.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( Application& app, QWidget* parent, QStackedWidget* pages, Page_ABC& previous, const frontend::Config& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, frontend::LauncherClient& launcher )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonEdit, launcher )
    , config_( config )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( app, pages, *this ) )
{
    setWindowTitle( "ScenarioEditPage" );
    QWidget* box = new QWidget( this );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    mainTabs_ = new QTabWidget( box );

    // eTabs_Edit
    exercises_ = new ExerciseList( app, parent, config_, fileLoader_, controllers, true, false );
    mainTabs_->addTab( exercises_, "" );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    connect( exercises_, SIGNAL( ExercisePropertiesChanged() ), SLOT( OnExercisePropertiesChanged() ) );

    // eTabs_Create
    createExerciceWidget_ = new CreateExerciceWidget( *this, box, config, fileLoader );
    mainTabs_->addTab( createExerciceWidget_, "" );

    boxLayout->setMargin( 5 );
    boxLayout->addWidget( mainTabs_ );
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
    SetTitle( tools::translate( "ScenarioEditPage", "Prepare" ) );
    progressPage_->SetTitle(  tools::translate( "ScenarioEditPage", "Editing exercise" ) );
    mainTabs_->setTabText( eTabs_Edit, tools::translate( "ScenarioEditPage", "Edit" ) );
    mainTabs_->setTabText( eTabs_Create, tools::translate( "ScenarioEditPage", "Create" ) );
    UpdateEditButton();
    LauncherClientPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ExerciceExists
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool ScenarioEditPage::ExerciceExists( const tools::Path& exercise )
{
    return exercises_->Exists( exercise );
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
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditExercise
// Created: SBO 2008-10-07
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnEdit()
{
    EnableButton( eButtonEdit, false );
    switch( mainTabs_->currentIndex() )
    {
    case eTabs_Edit:
        if( exercise_ && exercises_->ChangeExerciceParameters() )
            Edit( exercise_->GetName() );
        break;
    case eTabs_Create:
        createExerciceWidget_->CreateExercise();
        break;
    default:
        break;
    }
    EnableButton( eButtonEdit, true );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::Edit
// Created: SBO 2008-10-16
// -----------------------------------------------------------------------------
void ScenarioEditPage::Edit( const tools::Path& exercise )
{
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::EditExercise >( config_, exercise ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
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
    switch( mainTabs_->currentIndex() )
    {
    case eTabs_Edit:
        enable = exercise_ != 0 && exercises_->IsPropertiesValid();
        SetButtonText( eButtonEdit, tools::translate( "Page_ABC", "Edit" ) );
        break;

    case eTabs_Create:
        enable = createExerciceWidget_->EnableEditButton();
        SetButtonText( eButtonEdit, tools::translate( "ScenarioEditPage", "Create" ) );
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
// Name: ScenarioEditPage::OnExercisePropertiesChanged
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnExercisePropertiesChanged()
{
    UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::LaunchScenarioImport
// Created: LDC 2010-07-06
// LTO
// -----------------------------------------------------------------------------
void ScenarioEditPage::LaunchScenarioImport( const tools::Path& inputScenario, const tools::Path& outputScenario )
{
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::ImportExercise >( config_, inputScenario, outputScenario ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::LaunchPreparation
// Created: LDC 2010-07-06
// LTO
// -----------------------------------------------------------------------------
void ScenarioEditPage::LaunchPreparation( const tools::Path& outputScenario )
{
    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::EditExercise >( config_, outputScenario ) );
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}
