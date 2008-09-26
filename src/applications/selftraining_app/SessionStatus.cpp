// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionStatus.h"
#include "moc_SessionStatus.cpp" 
#include "MessageDialog.h" 
#include "clients_kernel/Tools.h" 
#include "frontend/SpawnCommand.h"
#pragma warning( push )
#pragma warning( disable : 4127 4244 4511 4512 )
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: SessionStatus constructor
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
SessionStatus::SessionStatus( frontend::SpawnCommand* simulation, frontend::SpawnCommand* gui )
    : simulation_ ( simulation ) 
    , gui_ ( gui ) 
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: SessionStatus destructor
// Created: RDS 2008-08-21
// -----------------------------------------------------------------------------
SessionStatus::~SessionStatus()
{
    if ( thread_.get() )
    {
        thread_->interrupt(); 
        simulation_.reset(); 
        gui_.reset(); 
        thread_->join(); 
    }
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void SessionStatus::Start()
{
    thread_.reset( new boost::thread( boost::bind( &SessionStatus::ThreadStart, this ) ) );     
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void SessionStatus::ThreadStart()
{
    try
    {
        if ( simulation_.get() ) 
        {
            emit ( Feedback ( "start_simulation" ) ) ; 
            boost::this_thread::interruption_point(); 
            simulation_->Start(); 
            boost::this_thread::interruption_point(); 
            simulation_->Wait(); 
            boost::this_thread::interruption_point(); 
            emit SimulationStarted(); 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "end_simulation" ) ) ; 
            boost::this_thread::interruption_point(); 
        }
        if ( gui_.get() ) 
        {
            gui_->Start(); 
            boost::this_thread::interruption_point(); 
            emit GUIStarted(); 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "start_gui" ) ) ; 
            boost::this_thread::interruption_point(); 
            gui_->Wait(); 
            boost::this_thread::interruption_point(); 
            emit GUIClosed(); 
            boost::this_thread::interruption_point(); 
            emit ( Feedback ( "end_gui" ) ) ; 
            boost::this_thread::interruption_point(); 
        }
    }
    catch(...)
    {

    }
}
// -----------------------------------------------------------------------------
// Name: SessionStatus::IsSimRunning
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool SessionStatus::IsSimRunning()
{
    return ( simulation_.get() && simulation_->IsRunning() )  ; 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::IsGUIRunning
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool SessionStatus::IsGUIRunning()
{
    return ( gui_.get() && gui_->IsRunning() )  ; 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::HasRunningProcess
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
bool SessionStatus::HasRunningProcess()
{
    return ( IsSimRunning() || IsGUIRunning() ) ; 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::StopSimulation
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void SessionStatus::StopSimulation( bool confirm )
{
    Stop( simulation_, confirm ) ; 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::StopGUI
// Created: RDS 2008-08-22
// -----------------------------------------------------------------------------
void SessionStatus::StopGUI( bool confirm )
{
    Stop( gui_, confirm ) ; 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::Stop
// Created: RDS 2008-08-25
// -----------------------------------------------------------------------------
void SessionStatus::Stop( std::auto_ptr< frontend::SpawnCommand >& command, bool confirm )
{
    if ( command.get() && confirm ) 
    {
        MessageDialog message( qApp->mainWidget(), tools::translate ( "SessionStatus", "Running Sessions")  , tools::translate( "SessionStatus","Running session detected. Close ?" ) , QMessageBox::Yes, QMessageBox::No );
        if( message.exec() != QMessageBox::Yes )
            return ; 
    }
    command.reset(); 
}

// -----------------------------------------------------------------------------
// Name: SessionStatus::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int SessionStatus::GetPercentage()
{
    return ( simulation_.get() ? simulation_->GetPercentage() : 0 ) ; 
}

