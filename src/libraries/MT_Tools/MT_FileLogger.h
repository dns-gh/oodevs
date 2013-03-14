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
#include "tools/Path.h"
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )

namespace tools
{
    class Ofstream;
}

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
    explicit MT_FileLogger( const tools::Path& strFileName, unsigned int maxFiles, int maxSize, int nLogLevels,
                            bool bClearPreviousLog = false, E_Type type = eSimulation, bool sizeInBytes = false );

    virtual ~MT_FileLogger();

protected:
    //-------------------------------------------------------------------------
    /** @name Main method */
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteString( const std::string& s );
    unsigned int OpenNewOfstream( const tools::Path& fileName, bool clearPrevious = true );
    unsigned int GetOldestFile();
    tools::Path GetFileName( unsigned int fileCount ) const;
    //@}

private:
    std::auto_ptr< tools::Ofstream > file_;
    tools::Path fileName_;
    tools::Path fileNameNoExtension_;
    tools::Path fileNameExtension_;
    boost::mutex mutex_;
    const unsigned int maxFiles_;
    unsigned int filesCount_;
    const int maxSize_;
    int sizeCount_;
    bool sizeInBytes_;
};

#endif // __MT_FileLogger_h_
