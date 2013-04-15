// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "RasterProcess.h"
#include "moc_RasterProcess.cpp"
#include "Config.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/EncodingConverter.h>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
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
    if( logfile_.Exists() )
    {
        tools::Ifstream fp( logfile_, std::ios::in );
        std::string line;
        while( std::getline( fp, line ))
        {
            MT_LOG_INFO_MSG( line );
        }
        fp.close();
        logfile_.Remove();
    }
    fired_ = true;
    callback_( code, output );
}

boost::shared_ptr< QProcess > RunRasterApp( const tools::Path& input, int pixelSize,
        const Config& config, const RasterCallback& callback )
{
    QStringList parameters;
    tools::Path cfgPath = config.BuildTerrainChildFile( "config.xml" )
        .SystemComplete();
    tools::Path output = config.GetGraphicsDirectory() / "~~tmp.texture.bin";
    tools::Path logpath = output + ".log";
    parameters << ( std::string( "--config=" ) + cfgPath.ToUTF8()).c_str();
    parameters << ( std::string( "--raster=" ) + input.ToUTF8()).c_str();
    parameters << ( std::string( "--pixelsize=" )
        + boost::lexical_cast< std::string >( pixelSize ) ).c_str();
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
