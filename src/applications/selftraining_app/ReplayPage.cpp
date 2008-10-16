// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ReplayPage.h"
#include "moc_ReplayPage.cpp"
#include "ExerciseList.h" 
#include "CompositeProcessWrapper.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "frontend/commands.h"
#include "frontend/CommandLineTools.h"
#include "frontend/CreateSession.h"
#include "frontend/StartReplay.h"
#include "frontend/JoinAnalysis.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h" 
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: ReplayPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::ReplayPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "ReplayPage", "Replay" ), previous )
    , config_( config )
    , controllers_( controllers ) 
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ReplayPage", "Starting replay session" ), controllers ) )
{
    QVBox* mainBox = new QVBox( this );
    {
        QHBox* hbox = new QHBox( mainBox );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        hbox->setSpacing( 10 );
        {
            exercises_ = new ExerciseList( hbox, config,"",false );
            connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) );
            connect( exercises_, SIGNAL( Highlight( const QString& ) ), this, SLOT( OnSelectExercise( const QString& ) ) );
        }
        {
            QVBox* vbox = new QVBox( hbox );
            vbox->setSpacing( 5 );
            vbox->setBackgroundOrigin( QWidget::WindowOrigin );
            QLabel* label = new QLabel( tools::translate( "ReplayPage", "Session:" ) , vbox );
            label->setBackgroundOrigin( QWidget::WindowOrigin );
            sessionList_ = new QListBox( vbox );
        }
    }
    AddContent( mainBox );
    AddNextButton( tools::translate( "ReplayPage", "Start" ), *this, SLOT( OnStart() ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::~ReplayPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::Update()
{
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnStartExercise
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ReplayPage::OnStartExercise( const QString& exercise )
{
    if( exercise.isEmpty() || ! dialogs::KillRunningProcesses( this ) )
        return;
    QString session = "";  
    if( QListBoxItem* sessionItem = sessionList_->selectedItem() )
        session = sessionItem->text();
    if( !session.isEmpty() )
    {
        const unsigned int port = frontend::DispatcherPort( 1 ); // $$$$ SBO 2008-10-16: hard coded exercise number
        ConfigureSession( exercise, session );
        boost::shared_ptr< frontend::SpawnCommand > replay( new frontend::StartReplay( config_, exercise, session, port, true ) );
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinAnalysis( config_, exercise, port, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new CompositeProcessWrapper( controllers_.controller_, replay, client ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void ReplayPage::OnStart()
{
    QString exercise = exercises_->GetHighlight() ; 
    if ( exercise != "" ) 
        OnStartExercise( exercise ) ; 
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::OnSelectExercise
// Created: RDS 2008-09-02
// -----------------------------------------------------------------------------
void ReplayPage::OnSelectExercise( const QString& exercise )
{
    sessionList_->clear(); 
    sessionList_->insertStringList( frontend::commands::ListSessions( config_, exercise.ascii() ) );
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::ConfigureSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void ReplayPage::ConfigureSession( const QString& exercise, const QString& session )
{
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues(); // $$$$ SBO 2008-10-16: erases specific parameters
    {
        // force the networklogger to be used 
        action.SetOption( "session/config/simulation/debug/@networklogger"     , true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport" , 20000 );
    }
}

