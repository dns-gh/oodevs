// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_LogManager.h"
#include <tools/Logger_ABC.h>
#include <boost/make_shared.hpp>

//-----------------------------------------------------------------------------
// Name: MT_LogManager constructor
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager::MT_LogManager()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager destructor
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager::~MT_LogManager()
{
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Instance()
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
MT_LogManager& MT_LogManager::Instance()
{
    static MT_LogManager instance;
    return instance;
}

void MT_LogManager::Reset()
{
    loggerSet_.clear();
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager::RegisterLogger
/**
    @param  logger Logger to register to the log manager
    @return The state of the operation
*/
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
bool MT_LogManager::RegisterLogger( MT_Logger_ABC& logger )
{
    return loggerSet_.insert( &logger ).second;
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager::UnregisterLogger
/**
    @param  logger Logger to unregister from the log manager
    @return The state of the operation
*/
// Created: NLD 2002-03-25
//-----------------------------------------------------------------------------
bool MT_LogManager::UnregisterLogger( MT_Logger_ABC& logger )
{
    return loggerSet_.erase( &logger ) == 1;
}

//-----------------------------------------------------------------------------
// Name: MT_LogManager::Log
/**
    @param  nLevel       Log level
    @param  strMessage   Message
    @param  strContext   Context
    @param  nCode        Code
*/
// Created:  NLD 00-06-05
//-----------------------------------------------------------------------------
void MT_LogManager::Log( MT_Logger_ABC::E_LogLevel nLevel, const char* strMessage, const char* strContext, int nCode )
{
    for( IT_LoggerSet itLogger = loggerSet_.begin(); itLogger != loggerSet_.end(); ++itLogger )
        (*itLogger)->Log( nLevel, strMessage, strContext, nCode );
}

namespace
{

MT_Logger_ABC::E_LogLevel GetLogLevel( tools::Logger_ABC::eLevel level )
{
    switch( level )
    {
        case tools::Logger_ABC::error: return MT_Logger_ABC::eLogLevel_Error;
        case tools::Logger_ABC::warning : return MT_Logger_ABC::eLogLevel_Warning;
        default: return MT_Logger_ABC::eLogLevel_Info;
    }
}

class TerrainLogger : public tools::Logger_ABC
{
public:
             TerrainLogger( const std::string& name ) : name_( name + ": " ) {}
    virtual ~TerrainLogger() {}

    virtual void Log( eLevel level, const char* msg )
    {
        MT_LogManager::Instance().Log( GetLogLevel( level ), (name_ + msg).c_str() );
    }

private:
    const std::string name_;
};

}  // namespace

boost::shared_ptr< tools::Logger_ABC > CreateMTLogger( const std::string& name )
{
    return boost::make_shared< TerrainLogger >( name );
}

