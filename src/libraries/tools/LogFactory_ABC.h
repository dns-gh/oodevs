// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_LogFactory_ABC_h
#define tools_LogFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <iosfwd>
#include <memory>
#include <string>

namespace tools
{
    class Path;

// =============================================================================
/** @class  LogFactory_ABC
    @brief  Log factory declaration
*/
// Created: MCO 2011-06-26
// =============================================================================
class LogFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogFactory_ABC() {}
    virtual ~LogFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::size_t Write( std::ostream& os, const std::string& line ) = 0;
    virtual std::streamoff ComputeSize( const tools::Path& filename ) const = 0;
    //@}
};
}

#endif // tools_LogFactory_ABC_h
