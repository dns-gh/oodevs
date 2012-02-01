// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
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

public:
    explicit MT_FileLogger( const char* strFileName, unsigned int maxFiles = 1, int maxSize = -1, int nLogLevels = eLogLevel_All, 
                            bool bClearPreviousLog = false, E_Type type = eSimulation );

    virtual ~MT_FileLogger();

protected:
    //-------------------------------------------------------------------------
    /** @name Main method */
    //-------------------------------------------------------------------------
    //@{
    virtual void LogString( E_LogLevel level, const char* strMessage, const char* strContext, int nCode );
    void OpenNewOfstream( const std::string fileName );
    //@}

private:
    std::auto_ptr< std::ofstream > file_;
    std::string fileName_;
    std::string fileNameNoExtension_;
    std::string fileNameExtension_;
    boost::mutex mutex_;
    unsigned int maxFiles_;
    unsigned int filesCount_;
    bool bClearPreviousLog_;
    int maxSize_;
    int sizeCount_;
};

#endif // __MT_FileLogger_h_
