// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "PhyLoader.h"
#include "ExerciseConfig.h"
#include "Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: PhyLoader constructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
PhyLoader::PhyLoader( const Path& physicalFile, const ExerciseConfig& config,
        const boost::shared_ptr< Loader_ABC >& loader )
    : loader_( loader )
    , config_( config )
{
    auto xis = loader_->LoadFile( physicalFile );
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
// Name: PhyLoader destructor
// Created: NLD 2011-02-28
// -----------------------------------------------------------------------------
PhyLoader::~PhyLoader()
{
    // NOTHING
}

PhyLoader::File PhyLoader::GetPhysicalXml( const std::string& rootTag, bool optional ) const
{
    File file;
    file.path = GetPhysicalChildFile( rootTag );
    auto it = cached_.find( rootTag );
    if( it == cached_.end() )
    {
        if( file.path.IsEmpty() )
        {
            if( !optional )
                throw MASA_EXCEPTION( "cannot load disallowed physical file entry: " + rootTag );
            return file;
        }
        if( !optional || file.path.Exists() )
        {
            const boost::shared_ptr< xml::xistream > cached( loader_->LoadFile( file.path ) );
            it = cached_.insert( std::make_pair( rootTag, cached )).first;
        }
    }
    if( it != cached_.end() )
        file.xml = it->second;
    return file;
}

Path PhyLoader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader, bool optional ) const
{
    const auto file = GetPhysicalXml( rootTag, optional );
    if( file.xml )
        loader( xml::xisubstream( *file.xml ));
    return file.path;
}

// -----------------------------------------------------------------------------
// Name: PhyLoader::LoadPhysicalFile
// Created: NLD 2010-02-23
// -----------------------------------------------------------------------------
Path PhyLoader::LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const
{
    return LoadPhysicalFile( rootTag, loader, false );
}

// -----------------------------------------------------------------------------
// Name: PhyLoader::LoadOptionalPhysicalFile
// Created: ABR 2011-05-24
// -----------------------------------------------------------------------------
Path PhyLoader::LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const
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

Path PhyLoader::GetPhysicalChildFile( const std::string& rootTag ) const
{
    return GetPhysicalChildEntry( rootTag, config_, allowedFiles_ );
}

Path PhyLoader::GetPhysicalChildPath( const std::string& rootTag ) const
{
    return GetPhysicalChildEntry( rootTag, config_, allowedPaths_ );
}
