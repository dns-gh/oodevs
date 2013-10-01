// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef tools_Log_h
#define tools_Log_h

#include "Log_ABC.h"
#include <boost/scoped_ptr.hpp>

namespace tools
{
    class Path;
    class RotatingLog;

// =============================================================================
/** @class  Log
    @brief  Log implementation
*/
// Created: MCO 2013-10-01
// =============================================================================
class Log : private Log_ABC
{
public:
             Log( const tools::Path& filename, std::size_t files, std::size_t size, bool truncate, bool sizeInBytes );
    virtual ~Log();

    void Write( const std::string& s );

private:
    virtual std::size_t Write( std::ostream& os, const std::string& line );
    virtual std::streamoff ComputeSize( const tools::Path& filename ) const;

private:
    bool sizeInBytes_;
    boost::scoped_ptr< RotatingLog > log_;
};

}

#endif // tools_Log_h
