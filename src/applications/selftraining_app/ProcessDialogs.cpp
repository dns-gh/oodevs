// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ProcessDialogs.h"
#include "MessageDialog.h"
#include "clients_gui/Tools.h"
#include "tools/ProcessList.h"

namespace
{
    bool HasRunningProcess()
    {
        tools::ProcessList processes;
        return processes.Contains( "simulation_app.exe" )
            || processes.Contains( "gaming_app.exe" )
            || processes.Contains( "replayer_app.exe" )
            || processes.Contains( "preparation_app.exe" );
    }

    void StopRunningProcess()
    {
        tools::ProcessList processes;
        processes.KillAll( "simulation_app.exe" );
        processes.KillAll( "gaming_app.exe" );
        processes.KillAll( "replayer_app.exe" );
        processes.KillAll( "preparation_app.exe" );
    }
}

namespace dialogs
{

// -----------------------------------------------------------------------------
// Name: KillRunningProcesses
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
bool KillRunningProcesses( QWidget* parent )
{
    if( !HasRunningProcess() )
        return true;
    MessageDialog message( parent, tools::translate( "ProcessDialogs", "Running training sessions" )
                                 , tools::translate( "ProcessDialogs", "Running training session detected. Close ?" )
                                 , QMessageBox::Yes, QMessageBox::No );
    if( message.exec() == QMessageBox::No )
        return false;
    StopRunningProcess();
    return true;
}

}
