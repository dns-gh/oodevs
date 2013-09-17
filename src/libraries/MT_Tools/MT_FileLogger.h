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
#include <tools/Path.h>
#include "tools/RotatingLog.h"
#include "tools/LogFactory_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )

namespace tools
{
    class Path;
    class Ofstream;
}

//=============================================================================
/**
    @class  MT_FileLogger
    @brief  File logger implementation
*/
// Created:  NLD 00-06-05
//=============================================================================
class MT_FileLogger : public MT_Logger_ABC, private tools::LogFactory_ABC
{
public:
    MT_FileLogger( const tools::Path& filename, std::size_t files, std::size_t size, int levels,
                   bool truncate = false, E_Type type = eSimulation, bool sizeInBytes = false );
    virtual ~MT_FileLogger();

private:
    virtual void WriteString( const std::string& s );
    virtual std::auto_ptr< tools::Log_ABC > CreateLog( const tools::Path& filename, std::streamoff& size );

    std::streamoff ComputeSize( const tools::Path& filename ) const;

private:
    bool truncate_;
    bool sizeInBytes_;
    boost::mutex mutex_;
    tools::RotatingLog log_;
};

#endif // __MT_FileLogger_h_
