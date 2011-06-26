// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_LogFactory_ABC_h_
#define __dispatcher_LogFactory_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace dispatcher
{
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
    virtual std::auto_ptr< Log_ABC > CreateLog( const std::string& filename ) = 0;
    //@}
};
}

#endif // __dispatcher_LogFactory_ABC_h_
