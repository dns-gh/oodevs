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
#include <log4cxx/logger.h>
#include <log4cxx/appenderskeleton.h>
#include <log4cxx/spi/loggingevent.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/helpers/classregistration.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace
{

MT_Logger_ABC::E_LogLevel GetLogLevel( LevelPtr level )
{
    int value = Level::OFF_INT;
    if( level )
        value = level->toInt();
    switch( value )
    {
        case Level::ALL_INT  : return MT_Logger_ABC::eLogLevel_All;
        case Level::FATAL_INT: return MT_Logger_ABC::eLogLevel_FatalError;
        case Level::ERROR_INT: return MT_Logger_ABC::eLogLevel_Error;
        case Level::WARN_INT : return MT_Logger_ABC::eLogLevel_Warning;
        case Level::INFO_INT : return MT_Logger_ABC::eLogLevel_Info;
        case Level::DEBUG_INT:
        case Level::TRACE_INT: return MT_Logger_ABC::eLogLevel_Debug;
    }
    return MT_Logger_ABC::eLogLevel_None;
}

}  // namespace

class MTLoggerAppender : public AppenderSkeleton
{
public:
    DECLARE_LOG4CXX_OBJECT(MTLoggerAppender)
    BEGIN_LOG4CXX_CAST_MAP()
            LOG4CXX_CAST_ENTRY( MTLoggerAppender )
            LOG4CXX_CAST_ENTRY_CHAIN( AppenderSkeleton )
    END_LOG4CXX_CAST_MAP()

    void append( const spi::LoggingEventPtr& event, log4cxx::helpers::Pool& )
    {
        auto level = GetLogLevel( event->getLevel() );
        std::string msg;
        log4cxx::helpers::Transcoder::encodeUTF8( event->getMessage(), msg );
        MT_LogManager::Instance().Log( level, msg.c_str() );
    }

    void close()
    {
        if( !closed )
            closed = true;
    }

    bool isClosed() const
    {
        return closed;
    }

    bool requiresLayout() const
    {
        return false;
    }

private:
    bool closed_;
};

IMPLEMENT_LOG4CXX_OBJECT( MTLoggerAppender )

typedef helpers::ObjectPtrT<MTLoggerAppender> MTLoggerAppenderPtr;

void InitializeTerrainLogger()
{
    MTLoggerAppenderPtr appender = new MTLoggerAppender();
    log4cxx::LoggerPtr rootLogger = log4cxx::Logger::getRootLogger();
    rootLogger->setLevel( log4cxx::Level::getAll() );
    rootLogger->addAppender( appender );
}

