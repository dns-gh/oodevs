// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Logger.h"
#include "MT_Tools/MT_Logger.h"
#include "pathfind/PathfindLogger.h"
#include "spatialcontainer/Logging.h"
#include <boost/make_shared.hpp>

using namespace spatialcontainer;

namespace
{

MT_Logger_ABC::E_LogLevel GetLogLevel( Logger_ABC::eLevel level )
{
    switch( level )
    {
        case Logger_ABC::error: return MT_Logger_ABC::eLogLevel_Error;
        case Logger_ABC::warning : return MT_Logger_ABC::eLogLevel_Warning;
        default: return MT_Logger_ABC::eLogLevel_Info;
    }
}

class MTTerrainLogger : public spatialcontainer::Logger_ABC
{
public:
             MTTerrainLogger( const std::string& name ) : name_( name + ": " ) {}
    virtual ~MTTerrainLogger() {}

    virtual void Log( eLevel level, const char* msg )
    {
        MT_LogManager::Instance().Log( GetLogLevel( level ), (name_ + msg).c_str() );
    }

private:
    const std::string name_;
};

}  // namespace

void InitializeTerrainLogger()
{
    pathfind::SetLogger( boost::make_shared< MTTerrainLogger >( "pathfind" ));
    spatialcontainer::SetLogger( boost::make_shared< MTTerrainLogger >( "spatialcontainer" ));
}

