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
Loader::Loader( const Path& physicalFile, const ExerciseConfig& config,
        RealFileLoaderObserver_ABC& observer )
    : DefaultLoader( observer )
    , config_      ( config )
{
    auto xis = fileLoader_->LoadFile( physicalFile, observer_ );
    *xis >> xml::start( "physical" )
        >> xml::list( [&]( const std::string&, const std::string& name, xml::xistream& x )
        {
            std::string path;
            x >> xml::optional >> xml::attribute( "file", path );
            if( !path.empty() )
            {
                allowedFiles_[ name ] = path;
                return;
            }
            x >> xml::optional >> xml::attribute( "path", path );
            if( !path.empty() )
                allowedPaths_[ name ] = path;

        } );
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
    const Path path = GetPhysicalChildFile( rootTag );
    if( path.IsEmpty() )
    {
        if( !optional )
            throw MASA_EXCEPTION( "cannot load disallowed physical file entry: " + rootTag );
        return path;
    }
    if( !optional || path.Exists() )
        LoadFile( path, loader );
    return path;
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

namespace
{

Path GetPhysicalChildEntry( const std::string& tag, const ExerciseConfig& config,
        const std::map< std::string, std::string >& entries )
{
    auto it = entries.find( tag );
    if( it == entries.end() )
        return Path();
    return config.BuildPhysicalChildFile( Path::FromUTF8( it->second ) );
}

} // namespace

Path Loader::GetPhysicalChildFile( const std::string& rootTag ) const
{
    return GetPhysicalChildEntry( rootTag, config_, allowedFiles_ );
}

Path Loader::GetPhysicalChildPath( const std::string& rootTag ) const
{
    return GetPhysicalChildEntry( rootTag, config_, allowedPaths_ );
}
