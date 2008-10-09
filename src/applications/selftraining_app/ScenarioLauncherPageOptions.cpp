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
#include "SessionRunningPage.h" 
#include "Session.h" 
#include "ProfileList.h"
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
ScenarioLauncherPageOptions::ScenarioLauncherPageOptions( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, SessionRunningPage& running, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPageOptions", "Options" ), previous )
    , controllers_ ( controllers )
    , running_ ( running )
    , config_ ( config )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->layout()->setAlignment(  Qt::AlignTop );
    box->setMargin( 5 );
    QLabel* label = new QLabel( tools::translate( "ScenarioLauncherPageOptions", "Profile:" ), box );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    profiles_ = new ProfileList( box, config );
    profiles_->setMaximumWidth( 300 );
    profiles_->setBackgroundOrigin( QWidget::WindowOrigin );
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
    const QString session  = BuildSessionName().c_str();
    CreateSession( exercise_, session );
    if ( profiles_->selectedItem() )
        running_.SetSession( new Session( controllers_.controller_, new frontend::StartExercise( config_, exercise_, session, true ), new frontend::JoinExercise( config_, exercise_, session, profiles_->selectedItem()->text(), true ) ) );
    else
        running_.SetSession( new Session( controllers_.controller_, new frontend::StartExercise( config_, exercise_, session, true ), new frontend::JoinExercise( config_, exercise_, session, true ) ) );
    running_.show();
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
        action.SetOption( "session/config/simulation/debug/@networklogger"     , true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport" , 20000 );
    }
}
