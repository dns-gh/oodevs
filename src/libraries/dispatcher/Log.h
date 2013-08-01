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

#include "tools/Log_ABC.h"
#include "tools/FileWrapper.h"
#include <tools/Path.h>
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
class Log : public tools::Log_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Log( const tools::Path& filename )
        : s_( filename )
    {
        if( ! s_ )
            throw MASA_EXCEPTION( "Failed to open log file '" + filename.ToUTF8() + "' for writing" );
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
    tools::Ofstream s_;
    //@}
};

}

#pragma warning( pop )

#endif // dispatcher_Log_h
