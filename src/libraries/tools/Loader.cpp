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
    auto xis = fileLoader_->LoadFile( config_.GetPhysicalFile(), observer_ );
    *xis >> xml::start( "physical" )
        >> xml::list( [&]( const std::string&, const std::string& name, xml::xistream& x )
        {
            std::string file;
            x >> xml::optional >> xml::attribute( "file", file );
            if( !file.empty() )
                allowedFiles_[ name ] = file;
        } )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Loader destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
Loader::~Loader()
{
    // NOTHING
}

Path Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader, bool optional ) const
{
    auto it = allowedFiles_.find( rootTag );
    if( it == allowedFiles_.cend() )
    {
        if( !optional )
            throw MASA_EXCEPTION( "cannot load disallowed physical file entry: " + rootTag );
        return Path();
    }
    const Path childFile = config_.BuildPhysicalChildFile( Path::FromUTF8( it->second ) );
    if( !optional || childFile.Exists() )
        LoadFile( childFile, loader );
    return childFile;
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Path Loader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    return LoadPhysicalFile( rootTag, loader, false );
}

// -----------------------------------------------------------------------------
// Name: Loader::LoadOptionalPhysicalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path Loader::LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    return LoadPhysicalFile( rootTag, loader, true );
}
