// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Loader.h"
#include "RealFileLoader.h"
#include "SchemaVersionExtractor.h"
#include "ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: Loader constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
Loader::Loader( const ExerciseConfig& config, RealFileLoaderObserver_ABC& observer )
    : DefaultLoader( observer )
    , config_      ( config )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
Loader::~Loader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Path Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    std::auto_ptr< xml::xistream > physicalFileXis = fileLoader_->LoadFile( config_.GetPhysicalFile(), observer_ );
    std::string childFileName;
    *physicalFileXis >> xml::start( "physical" )
                         >> xml::start( rootTag )
                             >> xml::attribute( "file", childFileName )
                         >> xml::end
                     >> xml::end;
    Path childFile = config_.BuildPhysicalChildFile( Path::FromUTF8( childFileName ) );
    LoadFile( childFile, loader );
    return childFile;
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadOptionalPhysicalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path Loader::LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    std::auto_ptr< xml::xistream > physicalFileXis = fileLoader_->LoadFile( config_.GetPhysicalFile(), observer_ );
    std::string childFileName;
    *physicalFileXis >> xml::start( "physical" )
                         >> xml::optional >> xml::start( rootTag )
                             >> xml::attribute( "file", childFileName )
                         >> xml::end
                     >> xml::end;
    if( childFileName.empty() )
        return Path();
    Path childFile = config_.BuildPhysicalChildFile( Path::FromUTF8( childFileName ) );
    if( childFile.Exists() )
        LoadFile( childFile, loader );
    return childFile;
}
