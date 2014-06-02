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
#include <terrain/PointProjector.h>
#include <terrain/GenConfig.h>
#include <tools/Helpers.h>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
#include <boost/make_shared.hpp>
#include <boost/assign/list_of.hpp>

namespace
{
    geometry::Rectangle2d MakeExtent( const tools::ExerciseConfig& config )
    {
        PointProjector projector( config.GetTerrainDir( config.GetTerrainName() ) );
        double latitude, longitude;
        projector.Unproject( geometry::Point2d( 0, 0 ), latitude, longitude );
        const geometry::Point2d bottomLeft( longitude, latitude );
        projector.Unproject( geometry::Point2d( config.GetTerrainWidth(), config.GetTerrainHeight() ), latitude, longitude );
        const geometry::Point2d topRight( longitude, latitude );
        return geometry::Rectangle2d( bottomLeft, topRight );
    }
}

RasterProcess::RasterProcess( const tools::Path& input,
    int pixelSize, const tools::ExerciseConfig& config,
    const RasterCallback& callback )
    : callback_( callback )
    , temp_( "raster-app-", config.GetGraphicsDirectory() )
    , config_( temp_.Path() / "config.xml" )
    , output_( temp_.Path() / "texture.bin" )
    , logfile_( output_ + ".log" )
    , fired_( false )
{
    {
        GenConfig conf;
        conf.SetOutputDirectory( temp_.Path() );
        conf.SetTerrainExtent( MakeExtent( config ) );
        conf.SetRasterSources( boost::assign::list_of( input ) );
        conf.AddProperty( "rasterPixelSize", pixelSize );
        conf.Flush( config_ );
    }
    connect( this, SIGNAL( finished( int, QProcess::ExitStatus ) ),
        SLOT( OnExit( int, QProcess::ExitStatus ) ) );
    connect( this, SIGNAL( error( QProcess::ProcessError ) ),
        SLOT( OnError( QProcess::ProcessError ) ) );
}

RasterProcess::~RasterProcess()
{
    // NOTHING
}

void RasterProcess::Start()
{
    const auto workingDirectory = tools::Path( "../Terrain/applications/" ).SystemComplete();
    const tools::Path exe = workingDirectory / "raster_app.exe";
    QStringList parameters;
    parameters << ( "--config=" + config_.SystemComplete().ToUTF8() ).c_str();
    parameters << ( "--file=" + output_.SystemComplete().ToUTF8() ).c_str();
    parameters << ( "--logfile=" + logfile_.SystemComplete().ToUTF8() ).c_str();
    std::stringstream cmd;
    cmd << exe.ToDebug() << " ";
    for( int i = 0; i != parameters.count(); ++i )
        cmd << tools::EscapeNonAscii( parameters.at( i ).toStdWString() ) << " ";
    MT_LOG_INFO_MSG( "Executing: " << cmd.str() );
    setWorkingDirectory( workingDirectory.ToUTF8().c_str() );
    start( exe.ToUTF8().c_str(), parameters );
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
    }
    fired_ = true;
    callback_( code, output, error );
}

boost::shared_ptr< QProcess > RunRasterApp( const tools::Path& input,
    int pixelSize, const tools::ExerciseConfig& config,
    const RasterCallback& callback )
{
    const auto process = boost::make_shared< RasterProcess >( input, pixelSize, config, callback );
    process->Start();
    return process;
}
