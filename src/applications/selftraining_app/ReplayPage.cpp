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
#include "MessageDialog.h" 
#include "MenuButton.h" 
#include "SessionRunningPage.h" 
#include "moc_ReplayPage.cpp"
#include "ExerciseList.h" 
#include "SideList.h"
#include "Session.h" 
#include "frontend/commands.h"
#include "frontend/CreateSession.h" 
#include "frontend/StartReplay.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/commands.h" 
#include "clients_gui/Tools.h"
#include "clients_gui/LinkInterpreter_ABC.h" 
#include "clients_kernel/Controllers.h" 
#include "tools/GeneralConfig.h"
#include <qlistbox.h>
#include <qtextbrowser.h> 
#include <qtabwidget.h> 
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qtabbar.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 
#pragma warning( pop )

namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;

namespace
{
    std::string BuildSessionName()
    {
        return bpt::to_iso_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: ReplayPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ReplayPage::ReplayPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config, SessionRunningPage& running, boost::shared_ptr< Session > sessionStatus  )
    : ContentPage( pages, tools::translate( "ReplayPage", "Replay" ), previous )
    , controllers_ ( controllers ) 
    , config_( config )
    , running_( running ) 
    , sessionStatus_( sessionStatus ) 
{
    QVBox* mainBox = new QVBox ( this ); 
    {
        {
            QHBox* hbox = new QHBox ( mainBox ) ; 
            hbox->setBackgroundOrigin( QWidget::WindowOrigin ); 
            hbox->setSpacing( 10 );  
            {
                exercises_ = new ExerciseList( hbox, config,"",false )  ; 
                connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) ); 
                connect( exercises_, SIGNAL( Highlight( const QString& ) ), this, SLOT( OnSelectExercise( const QString& ) ) ); 
            }
            {
                QLabel* label ; 
                QVBox* vbox = new QVBox( hbox ) ;
                vbox->setSpacing( 5 );
                vbox->setBackgroundOrigin( QWidget::WindowOrigin );
                label = new QLabel( tools::translate( "ReplayPage", "Session:" ) , vbox ); 
                label->setBackgroundOrigin( QWidget::WindowOrigin );
                sessionList_ = new QListBox( vbox);
            }
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
    if (sessionStatus_.get() && sessionStatus_->HasRunningProcess()) 
    {
        MessageDialog message( this, tools::translate( "ReplayPage", "Running Sessions" ), tools::translate( "ReplayPage", "Running session detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() != QMessageBox::Yes )
            return ; 
    }

    QString session = "";  
    if( QListBoxItem* sessionItem = sessionList_->selectedItem() )
        session = sessionItem->text();
    if ( session != "" ) 
    {
        CreateSession( exercise, session );
        running_.SetSession( new Session( controllers_.controller_, new frontend::StartReplay( config_, exercise, session, 20000, true ), new frontend::JoinAnalysis( config_, exercise, 20000 ) ) ); 
        running_.show();  
    }
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void ReplayPage::OnStart( )
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
// Name: ScenarioLauncherPage::StartSession
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void ReplayPage::StartSession( Session* session )
{
    sessionStatus_.reset( session ) ; 
    sessionStatus_->Start(); 
}

// -----------------------------------------------------------------------------
// Name: ReplayPage::CreateSession
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void ReplayPage::CreateSession( const QString& exercise, const QString& session )
{
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues();
    {
        // force the networklogger to be used 
        action.SetOption( "session/config/simulation/debug/@networklogger"     , true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport" , 20000 );
    }
}

