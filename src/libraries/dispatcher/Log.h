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
#include <ctime>
#pragma warning( push )
#pragma warning( disable: 4996 )

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
        s_ << "[" << GetTime() << "] " << s << std::endl;
    }
    //@}

private:
    //! @name Helpers
    //@{
    const char* GetTime()
    {
        static char buffer[256];
        time_t nTime = time( NULL );
        strftime( buffer, 256, "%H:%M:%S", localtime( &nTime ) );
        return buffer;
    }
    //@}

    //! @name Member data
    //@{
    std::ofstream s_;
    //@}
};

}

#pragma warning( pop )

#endif // dispatcher_Log_h
