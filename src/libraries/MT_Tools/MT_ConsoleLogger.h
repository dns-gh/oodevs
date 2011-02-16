// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MT_ConsoleLogger_h_
#define __MT_ConsoleLogger_h_

#include "MT_Logger_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )

//=============================================================================
/**
    @class  MT_ConsoleLogger
    @brief  PUT THE COMMENTS ON THE CLASS HERE
    @par    Using example
    @code
        MT_ConsoleLogger logConsole;

        MT_LogManager::Instance().RegisterLogger( logConsole );
        MT_LogManager::Instance().UnregisterLogger( logConsole );

    @endcode
*/
// Created:  NLD 00-06-05
//=============================================================================
class MT_ConsoleLogger : public MT_Logger_ABC
{

public:
    explicit MT_ConsoleLogger( int nLogLevels = eLogLevel_All );
    virtual ~MT_ConsoleLogger();

    void EnableTimeStamps( bool );

private:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    const char* GetColorFromLogLevel( unsigned int nLogLevel );

    virtual void LogString( E_LogLevel level, const char* strMessage, const char* strContext, int nCode );
    //@}

private:
    boost::mutex mutex_;
    bool bDumpTimeStamps_;
};

#endif // __MT_ConsoleLogger_h_
