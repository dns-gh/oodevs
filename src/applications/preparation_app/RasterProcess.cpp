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
#include <tools/Path.h>
#include <tools/EncodingConverter.h>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

RasterProcess::RasterProcess( const RasterCallback& callback, const tools::Path& output )
    : callback_( callback )
    , output_( output )
    , fired_( false )
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
    parameters << ( std::string( "--config=" ) + cfgPath.ToUTF8()).c_str();
    parameters << ( std::string( "--raster=" ) + input.ToUTF8()).c_str();
    parameters << ( std::string( "--pixelsize=" )
        + boost::lexical_cast< std::string >( pixelSize ) ).c_str();
    parameters << ( std::string( "--file=" ) + output.SystemComplete().ToUTF8() ).c_str();

    auto p = boost::make_shared< RasterProcess >( callback, output );
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
