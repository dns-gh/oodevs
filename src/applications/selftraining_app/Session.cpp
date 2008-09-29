// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Session.h"
#include "SessionStatus.h" 
#include "moc_Session.cpp" 
#include "MessageDialog.h" 
#include "clients_kernel/Tools.h" 
#include "clients_kernel/Controller.h" 
#include "frontend/SpawnCommand.h"
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Session constructor
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
Session::Session( kernel::Controller& controller, frontend::SpawnCommand* simulation, frontend::SpawnCommand* gui )
    : controller_ ( controller ) 
    , simulation_ ( simulation ) 
    , gui_ ( gui ) 
{
    controller.Create( *(Session*) this ) ; 
}


// -----------------------------------------------------------------------------
// Name: Session destructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
Session::~Session()
{
    if ( thread_.get() )
    {
        thread_->interrupt(); 
        simulation_.reset(); 
        gui_.reset(); 
        thread_->join(); 
    }
    controller_.Delete( *this ) ; 
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void Session::Start()
{
    thread_.reset( new boost::thread( boost::bind( &Session::ThreadStart, this ) ) );     
}

// -----------------------------------------------------------------------------
// Name: Session::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void Session::ThreadStart()
{
    try
    {
        if ( simulation_.get() ) 
        {
            controller_.Update( SessionStatus( SessionStatus::SIM_STARTED, *this ) ) ; 
            emit ( Feedback ( "start_simulation" ) ) ;
            boost::this_thread::interruption_point(); 
            simulation_->Start(); 
            boost::this_thread::interruption_point(); 
            simulation_->Wait(); 
            boost::this_thread::interruption_point(); 
            emit SimulationStarted(); 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "end_simulation" ) ) ;  
            controller_.Update( SessionStatus( SessionStatus::SIM_AVAILABLE, *this ) ) ; 
            boost::this_thread::interruption_point(); 
        }
        if ( gui_.get() ) 
        {
            gui_->Start(); 
            boost::this_thread::interruption_point(); 
            emit GUIStarted(); 
            controller_.Update( SessionStatus( SessionStatus::GUI_OPENED, *this ) ) ; 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "start_gui" ) ) ; 
            boost::this_thread::interruption_point(); 
            gui_->Wait(); 
            boost::this_thread::interruption_point(); 
            emit GUIClosed(); 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "end_gui" ) ) ; 
            controller_.Update( SessionStatus( SessionStatus::GUI_CLOSED, *this ) ) ; 
            boost::this_thread::interruption_point(); 
        }
    }
    catch(...)
    {

    }
}
// -----------------------------------------------------------------------------
// Name: Session::IsSimRunning
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool Session::IsSimRunning() const 
{
    return ( simulation_.get() && simulation_->IsRunning() )  ; 
}

// -----------------------------------------------------------------------------
// Name: Session::IsGUIRunning
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool Session::IsGUIRunning() const 
{
    return ( gui_.get() && gui_->IsRunning() )  ; 
}

// -----------------------------------------------------------------------------
// Name: Session::HasRunningProcess
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool Session::HasRunningProcess()
{
    return ( IsSimRunning() || IsGUIRunning() ) ; 
}

// -----------------------------------------------------------------------------
// Name: Session::StopSimulation
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void Session::StopSimulation( bool confirm )
{
    Stop( simulation_, confirm ) ; 
}

// -----------------------------------------------------------------------------
// Name: Session::StopGUI
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void Session::StopGUI( bool confirm )
{
    Stop( gui_, confirm ) ; 
}

// -----------------------------------------------------------------------------
// Name: Session::Stop
// Created: RDS 2008-08-25
// -----------------------------------------------------------------------------
void Session::Stop( std::auto_ptr< frontend::SpawnCommand >& command, bool confirm )
{
    if ( command.get() && confirm ) 
    {
        MessageDialog message( qApp->mainWidget(), tools::translate ( "Session", "Running Sessions")  , tools::translate( "Session","Running session detected. Close ?" ) , QMessageBox::Yes, QMessageBox::No );
        if( message.exec() != QMessageBox::Yes )
            return ; 
    }
    command.reset(); 
}

// -----------------------------------------------------------------------------
// Name: Session::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int Session::GetPercentage()
{
    return ( simulation_.get() ? simulation_->GetPercentage() : 0 ) ; 
}

