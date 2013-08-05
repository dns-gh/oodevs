// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_LogManager_h_
#define __MT_LogManager_h_

#include "MT_Logger_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <set>

namespace tools
{
    class Logger_ABC;
}

class MT_LogManager : private boost::noncopyable
{

public:
     MT_LogManager();
    ~MT_LogManager();

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    bool RegisterLogger  ( MT_Logger_ABC& logger );
    bool UnregisterLogger( MT_Logger_ABC& logger );

    void Log( MT_Logger_ABC::E_LogLevel nLevel, const char* strMessage, const char* strContext = 0, int nCode = -1 );
    void Reset();
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    static MT_LogManager& Instance();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::set< MT_Logger_ABC* > T_LoggerSet;
    typedef T_LoggerSet::iterator      IT_LoggerSet;
    //@}

private:
    T_LoggerSet loggerSet_;
};

boost::shared_ptr< tools::Logger_ABC > CreateMTLogger( const std::string& name );

#endif // __MT_LogManager_h_
