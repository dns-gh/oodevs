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
#include <tools/Exception.h>
#include <tools/Path.h>
#include <ctime>

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
    Log( const tools::Path& filename, bool sizeInBytes )
        : s_          ( filename )
        , sizeInBytes_( sizeInBytes )
    {
        if( ! s_ )
            throw MASA_EXCEPTION( "Failed to open log file '" + filename.ToUTF8() + "' for writing" );
    }
    //@}

    //! @name Operations
    //@{
    virtual std::size_t Write( const std::string& s )
    {
        const std::string time = GetTime();
        s_ << "[" << time << "] " << s << std::endl;
        return sizeInBytes_ ? time.size() + s.size() + 3 : 1;
    }
    //@}

private:
    //! @name Helpers
    //@{
    std::string GetTime() const
    {
        char buffer[256];
        std::time_t t = time( 0 );
        std::strftime( buffer, sizeof( buffer ), "%H:%M:%S", std::localtime( &t ) );
        return buffer;
    }
    //@}

private:
    //! @name Member data
    //@{
    tools::Ofstream s_;
    bool sizeInBytes_;
    //@}
};

}

#endif // dispatcher_Log_h
