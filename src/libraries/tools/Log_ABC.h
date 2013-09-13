// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_Log_ABC_h
#define tools_Log_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace tools
{
    class Path;

// =============================================================================
/** @class  Log_ABC
    @brief  Log declaration
*/
// Created: MCO 2011-06-26
// =============================================================================
class Log_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Log_ABC() {}
    virtual ~Log_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::size_t Write( const std::string& line ) = 0;

    virtual void Rename( const tools::Path& filename ) = 0;
    virtual void Delete() = 0;
    //@}
};

}

#endif // tools_Log_ABC_h
