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
#include "PropertyTree.h"
#include "runtime/Utf8.h"
#include "TaskHandler.h"
#include "UuidFactory.h"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4244 4245 )
#endif
#include <boost/crc.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace host;
using runtime::Utf8Convert;

namespace
{
std::string Checksum( const boost::filesystem::path& root )
{
    boost::crc_32_type sum;
    std::vector< char > buffer( UINT16_MAX );
    for( boost::filesystem::recursive_directory_iterator it( root ); it != boost::filesystem::recursive_directory_iterator(); ++it )
    {
        if( !boost::filesystem::is_regular_file( it.status() ) )
            continue;
        std::ifstream in( Utf8Convert( *it ), std::ifstream::binary );
        while( in.good() )
        {
            in.read( &buffer[0], buffer.size() );
            sum.process_bytes( &buffer[0], static_cast< size_t >( in.gcount() ) );
        }
    }
    const size_t size = sprintf( &buffer[0], "%08X", sum.checksum() );
    return std::string( &buffer[0], size );
}
}

struct host::SubPackage : public boost::noncopyable
{
    explicit SubPackage( size_t id, const boost::filesystem::path& root )
        : root_( root )
        , id_  ( id )
    {
        tree_.put( "id", id_ );
        checksum_ = TaskHandler< std::string >::Go( boost::bind( &Checksum, root_ ) );
    }

    virtual ~SubPackage()
    {
        // NOTHING
    }

    virtual boost::property_tree::ptree GetProperties() const
    {
        return tree_;
    }
    void Join()
    {
        checksum_.wait();
        if( checksum_.has_value() )
            tree_.put( "checksum", checksum_.get() );
    }

protected:
    const boost::filesystem::path root_;
    const size_t id_;
    boost::property_tree::ptree tree_;
    TaskHandler< std::string >::Future checksum_;
};

namespace
{
std::string Format( const std::time_t& time )
{
    char tmp[64];
    const size_t size = strftime( tmp, sizeof tmp, "%Y-%m-%d %H:%M:%S", localtime( &time ) );
    return std::string( tmp, size );
}

std::string GetDate( const boost::filesystem::path& file )
{
    return Format( boost::filesystem::last_write_time( file ) );
}

boost::filesystem::path PopFilename( boost::filesystem::path& path )
{
    boost::filesystem::path reply = path.filename();
    path.remove_filename();
    return reply;
}

std::string GetFilename( boost::filesystem::path path, const std::string& root )
{
    boost::filesystem::path reply;
    path.remove_filename();
    while( path.filename() != root )
        reply = PopFilename( path ) / reply;
    return Utf8Convert( reply );
}

template< typename T >
bool MaybeGet( T& dst, const boost::property_tree::ptree& tree, const std::string& key )
{
    const boost::optional< T > value = tree.get_optional< T >( key );
    if( value == boost::none )
        return false;
    dst = *value;
    return true;
}

template< typename T >
void MaybeCopy( T& dst, const std::string& dstKey, const T& src, const std::string& srcKey, const std::string& default = std::string() )
{
    const boost::optional< std::string > value = src.get_optional< std::string >( srcKey );
    if( value != boost::none )
        dst.put( dstKey, *value );
    else if( !default.empty() )
        dst.put( dstKey, default );
}

struct Model : public SubPackage
{
    Model( size_t id, const boost::filesystem::path& file )
        : SubPackage( id, boost::filesystem::path( file ).remove_filename().remove_filename() )
    {
        tree_.put( "type", "model" );
        tree_.put( "name", Utf8Convert( root_.filename() ) );
        tree_.put( "date", GetDate( file ) );
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const boost::filesystem::path& root, T& items, size_t& idx )
    {
        BOOST_FOREACH( const boost::filesystem::path& path, system.Glob( root / "data" / "models", L"decisional.xml" ) )
            items.push_back( boost::make_shared< Model >( ++idx, path ) );
    }
};

struct Terrain : public SubPackage
{
    Terrain( size_t id, const boost::filesystem::path& file )
        : SubPackage( id, boost::filesystem::path( file ).remove_filename() )
    {
        tree_.put( "type", "terrain" );
        tree_.put( "name", GetFilename( file, "terrains" ) );
        tree_.put( "date", GetDate( file ) );
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const boost::filesystem::path& root, T& items, size_t& idx )
    {
        BOOST_FOREACH( const boost::filesystem::path& path, system.Glob( root / "data" / "terrains", L"Terrain.xml" ) )
            items.push_back( boost::make_shared< Terrain >( ++idx, path ) );
    }
};

struct Exercise : public SubPackage
{
    Exercise( size_t id, const FileSystem_ABC& system, const boost::filesystem::path& file )
        : SubPackage( id, boost::filesystem::path( file ).remove_filename() )
    {
        tree_.put( "type", "exercise" );
        tree_.put( "name", GetFilename( file, "exercises" ) );
        tree_.put( "date", GetDate( file ) );
        boost::property_tree::ptree more = FromXml( system.ReadFile( file ) );
        MaybeCopy( tree_, "name", more, "exercise.meta.name" );
        MaybeCopy( tree_, "briefing", more, "exercise.meta.briefing.text" );
        MaybeCopy( tree_, "terrain", more, "exercise.terrain.<xmlattr>.name" );
        MaybeCopy( tree_, "model", more, "exercise.model.<xmlattr>.dataset" );
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const boost::filesystem::path& root, T& items, size_t& idx )
    {
        BOOST_FOREACH( const boost::filesystem::path& path, system.Glob( root / "exercises", L"exercise.xml" ) )
            items.push_back( boost::make_shared< Exercise >( ++idx, system, path ) );
    }
};
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
    tree.put_child( "items", boost::property_tree::ptree() );
    boost::property_tree::ptree& items = tree.get_child( "items" );
    BOOST_FOREACH( const T_Packages::value_type& item, items_ )
        items.push_back( std::make_pair( "", item->GetProperties() ) );

    return tree;
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
        tree = FromXml( system_.ReadFile( index) );
    }
    catch( const std::exception& /*err*/ )
    {
        return false;
    }

    std::string name, description;

    bool valid = true;
    valid &= MaybeGet( name, tree, "content.name" );
    valid &= MaybeGet( description, tree, "content.description" );
    if( !valid )
        return false;

    name_ = name;
    description_ = description;
    MaybeGet( version_, tree, "content.version" );
    if( version_.empty() )
        version_ = "Unversioned";

    items_.clear();
    size_t idx = 0;
    Model::Parse( system_, path_, items_, idx );
    Terrain::Parse( system_, path_, items_, idx );
    Exercise::Parse( system_, path_, items_, idx );
    BOOST_FOREACH( T_Packages::value_type value, items_ )
        value->Join();

    valid_ = true;
    return true;
}