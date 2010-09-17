// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MT_FileLogger_h_
#define __MT_FileLogger_h_

#include "MT_Logger_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <fstream>

//=============================================================================
/**
    @class  MT_FileLogger
    @brief  PUT THE COMMENTS ON THE CLASS HERE
    @par    Using example
    @code
        MT_FileLogger logFile( "file.log" );

        MT_LogManager::Instance().RegisterLogger( logFile );    

        MT_LOG_INFO_MSG( "test msg" );

        MT_LogManager::Instance().UnregisterLogger( logFile );
    @endcode
*/
// Created:  NLD 00-06-05 
//=============================================================================
class MT_FileLogger : public MT_Logger_ABC
{
    MT_COPYNOTALLOWED( MT_FileLogger );

public:
    explicit MT_FileLogger( const char* strFileName
                          , uint nLogLevels = eLogLevel_All
                          , uint nLogLayers = eLogLayer_All
                          , bool bClearPreviousLog = false );

    virtual ~MT_FileLogger();

protected:
    //-------------------------------------------------------------------------
    /** @name Main method */
    //-------------------------------------------------------------------------
    //@{
    virtual void LogString( const char* strLayerName, E_LogLevel level, const char* strMessage, const char* strContext, int nCode );
    //@}

private:
    std::ofstream file_;
    boost::mutex mutex_;
};

#endif // __MT_FileLogger_h_
