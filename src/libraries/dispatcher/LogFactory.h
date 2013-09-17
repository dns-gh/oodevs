// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_LogFactory_h
#define dispatcher_LogFactory_h

#include "tools/LogFactory_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  LogFactory
    @brief  Log definition
*/
// Created: MCO 2011-06-27
// =============================================================================
class LogFactory : public tools::LogFactory_ABC
{
public:
    explicit LogFactory( bool sizeInBytes );

    virtual std::size_t Write( std::ostream& os, const std::string& line );
    virtual std::streamoff ComputeSize( const tools::Path& filename ) const;

private:
    bool sizeInBytes_;
};

}

#endif // dispatcher_LogFactory_h
