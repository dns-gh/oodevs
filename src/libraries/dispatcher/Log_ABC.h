// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Log_ABC_h_
#define __dispatcher_Log_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace dispatcher
{
    class Log_ABC;

// =============================================================================
/** @class  Log_ABC
    @brief  Log declaration
*/
// Created: MCO 2011-06-26
// =============================================================================
class Log_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Log_ABC() {}
    virtual ~Log_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( const std::string& ) = 0;
    //@}
};
}

#endif // __dispatcher_Log_ABC_h_
