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
#include "Log.h"

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
    //! @name Operations
    //@{
public:
    virtual std::auto_ptr< tools::Log_ABC > CreateLog( const tools::Path& filename )
    {
        return std::auto_ptr< tools::Log_ABC >( new Log( filename ) );
    }
    //@}
};

}

#endif // dispatcher_LogFactory_h
