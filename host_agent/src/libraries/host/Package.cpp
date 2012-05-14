// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Package.h"

#include "FileSystem_ABC.h"
#include "Json.h"
#include "runtime/Utf8.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace host;

// -----------------------------------------------------------------------------
// Name: SubPackage::SubPackage
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
SubPackage::SubPackage( const boost::filesystem::path& root )
    : name_    ( runtime::Utf8Convert( root.filename() ) )
    , date_    ( "" )
    , checksum_( "" )
    , action_  ( "" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SubPackage::~SubPackage
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
SubPackage::~SubPackage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Package::Package
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Package::Package( const FileSystem_ABC& system, const boost::filesystem::path& path )
    : system_( system )
    , path_  ( path )
    , valid_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Package::~Package
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Package::~Package()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Package::GetProperties
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
boost::property_tree::ptree Package::GetProperties() const
{
    boost::property_tree::ptree tree;
    if( !valid_ )
        return tree;

    tree.put( "name", name_ );
    tree.put( "description", description_ );
    tree.put( "version", version_ );
    return tree;
}

namespace
{
template< typename T >
bool MaybeGet( T& dst, const boost::property_tree::ptree& tree, const std::string& key )
{
    const boost::optional< T > value = tree.get_optional< T >( key );
    if( value == boost::none )
        return false;
    dst = *value;
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: Package::Parse
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
bool Package::Parse()
{
    const boost::filesystem::path index = path_ / "content.xml";
    if( !system_.IsFile( index ) )
        return false;

    boost::property_tree::ptree tree;
    try
    {
        const std::string data = system_.ReadFile( index);
        std::istringstream input( data );
        boost::property_tree::xml_parser::read_xml( input, tree );
    }
    catch( const std::exception& /*err*/ )
    {
        return false;
    }

    bool valid = true;
    valid &= MaybeGet( name_, tree, "content.name" );
    valid &= MaybeGet( description_, tree, "content.description" );
    if( !valid )
        return false;

    MaybeGet( version_, tree, "content.version" );

    ParseModels();
    ParseTerrains();
    ParseExercises();

    valid_ = true;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Package::ParseModels
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Package::ParseModels()
{
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Walk( path_ / "data" / "models", false ) )
        if( system_.IsDirectory( path ) )
            models.push_back( SubPackage( path ) );
}

// -----------------------------------------------------------------------------
// Name: Package::ParseTerrains
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Package::ParseTerrains()
{
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Walk( path_ / "data" / "terrains", false ) )
        if( system_.IsDirectory( path ) )
            terrains.push_back( SubPackage( path ) );
}

// -----------------------------------------------------------------------------
// Name: Package::ParseExercises
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Package::ParseExercises()
{
    BOOST_FOREACH( const boost::filesystem::path& path, system_.Walk( path_ / "exercises", false ) )
        if( system_.IsDirectory( path ) )
            exercises.push_back( SubPackage( path ) );
}