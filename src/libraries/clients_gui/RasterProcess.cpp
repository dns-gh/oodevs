// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RasterProcess.h"
#include "moc_RasterProcess.cpp"
#include "MT_Tools/MT_Logger.h"
#include "tools/ExerciseConfig.h"
#include <tools/Helpers.h>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
#include <tools/TemporaryDirectory.h>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

RasterProcess::RasterProcess( const RasterCallback& callback, const tools::Path& output,
       const tools::Path& logfile )
    : callback_( callback )
    , output_( output )
    , fired_( false )
    , logfile_( logfile )
{
    connect( this, SIGNAL( finished( int, QProcess::ExitStatus ) ),
        SLOT( OnExit( int, QProcess::ExitStatus ) ) );
    connect( this, SIGNAL( error( QProcess::ProcessError ) ),
        SLOT( OnError( QProcess::ProcessError ) ) );
}

RasterProcess::~RasterProcess()
{
    // NOTHING
}

void RasterProcess::OnExit( int errorCode, QProcess::ExitStatus exitStatus )
{
    int code = errorCode == EXIT_SUCCESS
        && exitStatus == QProcess::NormalExit ? 0 : 1;
    Fire( code, output_ );
}

void RasterProcess::OnError( QProcess::ProcessError )
{
    Fire( EXIT_FAILURE, output_ );
}

void RasterProcess::Fire( int code, const tools::Path& output )
{
    if( fired_ )
        return;
    std::string error;
    if( logfile_.Exists() )
    {
        tools::Ifstream fp( logfile_, std::ios::in );
        std::string line;
        while( std::getline( fp, line ) )
        {
            const auto message = line.substr( 7 );
            if( line.find( "ERROR" ) == 0 )
            {
                error = message;
                MT_LOG_ERROR_MSG( message );
            }
            else if( line.find( "WARN" ) == 0 )
                MT_LOG_WARNING_MSG( message );
            else
                MT_LOG_INFO_MSG( message );
        }
        fp.close();
        logfile_.Remove();
    }
    fired_ = true;
    callback_( code, output, error );
    try
    {
        output.Remove();
    }
    catch( const std::exception& )
    {
    }
}

boost::shared_ptr< QProcess > RunRasterApp( const tools::Path& input, int pixelSize,
        const tools::ExerciseConfig& config, const RasterCallback& callback )
{
    QStringList parameters;
    tools::Xifstream xis( config.BuildTerrainChildFile( "config.xml" ) );
    xis >> xml::start( "configuration" );
    tools::TemporaryDirectory tempDir( "raster-app", config.GetGraphicsDirectory() );
    const tools::Path cfgPath = tempDir.Path() / "~~tmp.config.xml";
    tools::Xofstream xos( cfgPath );
    xos << xml::start( "configuration" )
            << xml::start( "output" )
                << xml::content( "directory", config.GetGraphicsDirectory().Parent().SystemComplete() )
            << xml::end
            << xml::content( "extent", xml::xisubstream( xis ) >> xml::start( "extent" ) )
            << xml::start( "datasources" )
                << xml::start( "raster" )
                    << xml::content( "source", input )
                << xml::end
                << xml::start( "elevation" )
                << xml::end
                << xml::start( "vector" )
                << xml::end
            << xml::end
            << xml::start( "process" )
                << xml::start( "properties" )
                    << xml::start( "property" )
                        << xml::attribute( "name", "rasterPixelSize" )
                        << xml::attribute( "value", pixelSize )
                    << xml::end
                << xml::end
            << xml::end;
    const tools::Path output = tempDir.Path() / "~~tmp.texture.bin";
    const tools::Path logpath = output + ".log";
    parameters << ( std::string( "--config=" ) + cfgPath.SystemComplete().ToUTF8()).c_str();
    parameters << ( std::string( "--file=" ) + output.SystemComplete().ToUTF8() ).c_str();
    parameters << ( std::string( "--logfile=" ) + logpath.SystemComplete().ToUTF8() ).c_str();

    auto p = boost::make_shared< RasterProcess >( callback, output, logpath );
    auto workingDirectory = tools::Path( "../Terrain/applications/" ).SystemComplete();
    p->setWorkingDirectory( workingDirectory.ToUTF8().c_str() );
    tools::Path exePath = workingDirectory / "raster_app.exe";

    std::stringstream cmd;
    cmd << exePath.ToDebug() << " ";
    for( int i = 0; i != parameters.count(); ++i )
        cmd << tools::EscapeNonAscii( parameters.at( i ).toStdWString() ) << " ";
    MT_LOG_INFO_MSG( "Executing: " << cmd.str() );

    p->start( exePath.ToUTF8().c_str(), parameters );
    return p;
}
