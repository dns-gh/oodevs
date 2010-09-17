// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
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
    MT_COPYNOTALLOWED( MT_ConsoleLogger );

public:

public:
    explicit MT_ConsoleLogger( uint nLogLevels = eLogLevel_All, uint nLayers = eLogLayer_All );
    virtual ~MT_ConsoleLogger();

    void EnableTimeStamps( bool );

private:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    const char* GetColorFromLogLevel( uint nLogLevel );

    virtual void LogString( const char* strLayerName, E_LogLevel level, const char* strMessage, const char* strContext, int nCode );
    //@}

private:
    boost::mutex mutex_;
    bool bDumpTimeStamps_;
};

#endif // __MT_ConsoleLogger_h_
