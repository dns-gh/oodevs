// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Logger_ABC_h_
#define __dispatcher_Logger_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace dispatcher
{
// =============================================================================
/** @class  Logger_ABC
    @brief  Logger definition
*/
// Created: SBO 2011-05-19
// =============================================================================
class Logger_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Logger_ABC() {}
    virtual ~Logger_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void LogInfo( const std::string& message ) = 0;
    virtual void LogError( const std::string& message ) = 0;
    virtual void LogWarning( const std::string& message ) = 0;
    //@}
};
}

#endif // __dispatcher_Logger_ABC_h_
