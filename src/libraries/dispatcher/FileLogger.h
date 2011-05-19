// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_FileLogger_h_
#define __dispatcher_FileLogger_h_

#include "Logger_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <fstream>

namespace dispatcher
{
// =============================================================================
/** @class  FileLogger
    @brief  File logger
*/
// Created: SBO 2011-05-19
// =============================================================================
class FileLogger : public Logger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FileLogger( const std::string& filename );
    virtual ~FileLogger();
    //@}

    //! @name Operations
    //@{
    virtual void LogInfo( const std::string& message );
    virtual void LogError( const std::string& message );
    virtual void LogWarning( const std::string& message );
    //@}

private:
    //! @name Helpers
    //@{
    void LogMessage( const std::string& severity, const std::string& message );
    //@}

private:
    //! @name Member data
    //@{
    std::ofstream output_;
    boost::mutex mutex_;
    //@}
};
}

#endif // __dispatcher_FileLogger_h_
