// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <cassert>
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"
#include "dispatcher/Dispatcher.h"

using namespace dispatcher;

//-----------------------------------------------------------------------------
// Name: main constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int main( int nArgc, char* pArgv[] )
{
//    std::string strMsg = "Scipio SIM - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;
//    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
//    MT_LOG_STARTUP_MESSAGE( strMsg.c_str() );
//    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    Dispatcher dispatcher;

    while( 1 )
        dispatcher.Update();


    //     __try
//    {
//        return Run( nArgc, pArgv );
//    }
//    __except( WriteMiniDump( GetExceptionInformation() ) )
//    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
//    {
//    }

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
}
