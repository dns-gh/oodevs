// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioLauncherPageOptions.h"
#include "moc_ScenarioLauncherPageOptions.cpp"
#include "ProfileList.h"
#include "ProgressPage.h"
#include "CompositeProcessWrapper.h"
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/CreateSession.h"
#include "clients_kernel/Controllers.h" 
#include "clients_gui/Tools.h"

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPageOptions constructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
ScenarioLauncherPageOptions::ScenarioLauncherPageOptions( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPageOptions", "Options" ), previous )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioLauncherPageOptions", "Starting session" ), controllers ) )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->layout()->setAlignment(  Qt::AlignTop );
    box->setMargin( 5 );
    QLabel* label = new QLabel( tools::translate( "ScenarioLauncherPageOptions", "Profile:" ), box );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QHBox* hbox = new QHBox( box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        profiles_ = new ProfileList( hbox, config );
        profiles_->setMaximumWidth( 300 );
        profiles_->setBackgroundOrigin( QWidget::WindowOrigin );
        label = new QLabel( tools::translate( "ScenarioLauncherPageOptions", "The 'anonymous' profile is a default profile allowing to play the automated exercise when it is available." ), hbox );
        label->setAlignment( Qt::WordBreak );
        label->setMaximumWidth( 300 );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
    }
    AddContent( box );    
    AddNextButton( tools::translate( "ScenarioLauncherPageOptions", "Start" ) , *this, SLOT( Start() ) );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPageOptions destructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
ScenarioLauncherPageOptions::~ScenarioLauncherPageOptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPageOptions::Update
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPageOptions::Update( const QString& exercise )
{
    exercise_ = exercise;
    profiles_->Update( exercise ) ;
}

namespace
{
    std::string BuildSessionName()
    {
        return bpt::to_iso_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPageOptions::Start
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPageOptions::Start()
{
    const QString session = BuildSessionName().c_str();
    CreateSession( exercise_, session );
    boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exercise_, session, true ) );
    boost::shared_ptr< frontend::SpawnCommand > client;
    if( profiles_->selectedItem() && profiles_->selectedItem()->text() != tools::translate( "ReadProfile", "anonymous" ) )
        client.reset( new frontend::JoinExercise( config_, exercise_, session, profiles_->selectedItem()->text(), true ) );
    else
        client.reset( new frontend::JoinExercise( config_, exercise_, session, true ) );
    boost::shared_ptr< frontend::Process_ABC > process( new CompositeProcessWrapper( controllers_.controller_, simulation, client ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPageOptions::CreateSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPageOptions::CreateSession( const QString& exercise, const QString& session )
{
    frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
    action.SetDefaultValues();
    {
        // force the networklogger to be used
        action.SetOption( "session/config/simulation/debug/@networklogger", true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", 20000 );
    }
}
