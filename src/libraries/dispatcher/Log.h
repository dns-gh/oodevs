// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_Log_h
#define dispatcher_Log_h

#include "Log_ABC.h"

namespace dispatcher
{
// =============================================================================
/** @class  Log
    @brief  Log definition
*/
// Created: MCO 2011-06-27
// =============================================================================
class Log : public Log_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Log( const std::string& filename )
        : s_( filename.c_str() )
    {
        if( ! s_ )
            throw std::runtime_error( "Failed to open log file '" + filename + "' for writing" );
    }

    virtual ~Log()
    {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( const std::string& s )
    {
        s_ << s << std::endl;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::ofstream s_;
    //@}
};

}

#endif // dispatcher_Log_h
