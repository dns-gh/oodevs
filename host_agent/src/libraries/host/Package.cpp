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

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace host;
using runtime::Utf8Convert;

struct Package_ABC::Item_ABC : public boost::noncopyable
{
    virtual Tree        GetProperties() const = 0;
    virtual std::string GetType() const = 0;
    virtual std::string GetName() const = 0;
    virtual Path        GetSuffix() const = 0;
    virtual std::string GetChecksum() const = 0;
    virtual bool        Compare( size_t id ) const = 0;
    virtual bool        Compare( const Item_ABC& item ) const = 0;
    virtual void        Identify( const Package_ABC& ref, const Package_ABC& root ) = 0;
    virtual void        Join() = 0;
    virtual void        Install( const FileSystem_ABC& system, const Path& output, const Package_ABC& dst, const std::vector< Item_ABC* >& targets ) const = 0;
};

std::string GetVersion( const FileSystem_ABC& system, const Path& root, const boost::optional< std::string >& version )
{
    if( version != boost::none )
        return *version;
    const std::string& data = system.ReadFile( root / "version.tag" );
    if( data.empty() )
        return "Unversioned";
    const boost::optional< std::string > next = FromJson( data ).get_optional< std::string >( "version" );
    return next == boost::none ? "Unversioned" : *next;
}

namespace
{
struct Item : Package_ABC::Item_ABC
{
    Item( const FileSystem_ABC& system, const Path& root, size_t id, const std::string& name, const boost::optional< std::string >& version )
        : root_( root )
        , id_  ( id )
        , name_( name )
    {
        tree_.put( "id", id_ );
        if( !root_.empty() )
            future_ = TaskHandler< std::string >::Go( boost::bind( &FileSystem_ABC::Checksum, &system, root_, "version.tag" ) );
        tree_.put( "version", GetVersion( system, root_, version ) );
    }

    virtual ~Item()
    {
        // NOTHING
    }

    virtual Tree GetProperties() const
    {
        return tree_;
    }

    virtual std::string GetName() const
    {
        return name_;
    }

    virtual std::string GetChecksum() const
    {
        return checksum_;
    }

    virtual bool Compare( size_t id ) const
    {
        return id == id_;
    }

    virtual bool Compare( const Item_ABC& item ) const
    {
        return name_ == item.GetName() && GetType() == item.GetType();
    }

    typedef std::vector< boost::shared_ptr< Item > > T_Dependencies;
    virtual T_Dependencies GetDependencies() const
    {
        return T_Dependencies();
    }

    virtual void Identify( const Package_ABC& ref, const Package_ABC& root )
    {
        BOOST_FOREACH( const boost::shared_ptr< Item >& dep, GetDependencies() )
            if( !ref.Find( *dep ) && !root.Find( *dep ) )
            {
                tree_.put( "action", "error" );
                tree_.put( "error", "Missing " + dep->GetType() + " " + dep->name_ );
                return;
            }
        Item_ABC* iref = ref.Find( *this );
        if( !iref )
            tree_.put( "action", "add" );
        else if( checksum_ != iref->GetChecksum() )
            tree_.put( "action", "update" );
    }

    void Join()
    {
        future_.wait();
        if( !future_.has_value() )
            return;
        checksum_ = future_.get();
        tree_.put( "checksum", checksum_ );
    }

    virtual bool IsContained( const std::vector< Item_ABC* > items )
    {
        BOOST_FOREACH( const Item_ABC* it, items )
            if( Compare( *it ) )
                return true;
        return false;
    }

    void Install( const FileSystem_ABC& system, const Path& output, const Package_ABC& dst, const std::vector< Item_ABC* >& targets ) const
    {
        BOOST_FOREACH( const boost::shared_ptr< Item >& dep, GetDependencies() )
            if( !dst.Find( *dep ) && !dep->IsContained( targets ) )
                return;
        const Path& next = output / GetSuffix();
        system.Remove( next );
        system.MakeDirectory( next );
        system.CopyDirectory( root_, next );
        Tree data;
        data.put( "version", tree_.get< std::string >( "version" ) );
        system.WriteFile( next / "version.tag", ToJson( data ) );
    }

protected:
    const Path root_;
    const size_t id_;
    const std::string name_;
    Tree tree_;
    TaskHandler< std::string >::Future future_;
    std::string checksum_;
};

std::string Format( const std::time_t& time )
{
    char tmp[64];
    const size_t size = strftime( tmp, sizeof tmp, "%Y-%m-%d %H:%M:%S", localtime( &time ) );
    return std::string( tmp, size );
}

std::string GetDate( const Path& file )
{
    return Format( boost::filesystem::last_write_time( file ) );
}

Path PopFilename( Path& path )
{
    Path reply = path.filename();
    path.remove_filename();
    return reply;
}

std::string GetFilename( Path path, const std::string& root )
{
    Path reply;
    path.remove_filename();
    while( path.filename() != root )
        reply = PopFilename( path ) / reply;
    return Utf8Convert( reply );
}

void MaybePut( Tree& dst, const std::string& key, const std::string& value )
{
    if( !value.empty() )
        dst.put( key, value );
}

template< typename T >
void MaybeGet( T& dst, const Tree& tree, const std::string& key )
{
    const boost::optional< T > value = tree.get_optional< T >( key );
    if( value == boost::none )
        return;
    dst = *value;
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

struct Model : public Item
{
    Model( const FileSystem_ABC& system, const Path& file, size_t id, const boost::optional< std::string >& version )
        : Item( system, Path( file ).remove_filename().remove_filename(), id, Utf8Convert( root_.filename() ), version )
    {
        tree_.put( "type", GetType() );
        tree_.put( "name", name_ );
        tree_.put( "date", GetDate( file ) );
    }

    std::string GetType() const
    {
        return "model";
    }

    Path GetSuffix() const
    {
        return Path( "data" ) / "models" / name_;
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const boost::optional< std::string >& version )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "models", L"decisional.xml" ) )
            items.push_back( boost::make_shared< Model >( system, path, ++idx, version ) );
    }
};

struct Terrain : public Item
{
    Terrain( const FileSystem_ABC& system, const Path& file, size_t id, const boost::optional< std::string >& version )
        : Item ( system, Path( file ).remove_filename(), id, GetFilename( file, "terrains" ), version )
    {
        tree_.put( "type", GetType() );
        tree_.put( "name", name_ );
        tree_.put( "date", GetDate( file ) );
    }

    std::string GetType() const
    {
        return "terrain";
    }

    Path GetSuffix() const
    {
        return Path( "data" ) / "terrains" / name_;
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const boost::optional< std::string >& version )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "terrains", L"Terrain.xml" ) )
            items.push_back( boost::make_shared< Terrain >( system, path, ++idx, version ) );
    }
};

struct Dependency : public Item
{
    Dependency( const FileSystem_ABC& system, const std::string& type, const std::string& name )
        : Item ( system, "", 0, name, boost::none )
        , type_( type )
    {
        // NOTHING
    }

    std::string GetType() const
    {
        return type_;
    }

    Path GetSuffix() const
    {
        return Path();
    }

    const std::string type_;
};

struct Exercise : public Item
{
    Exercise( const FileSystem_ABC& system, const Path& file, size_t id, const boost::optional< std::string >& version, const Tree& more )
        : Item    ( system, Path( file ).remove_filename(), id, GetFilename( file, "exercises" ), version )
        , system_ ( system )
        , model_  ( GetDependency( more, "exercise.model.<xmlattr>.dataset" ) )
        , terrain_( GetDependency( more, "exercise.terrain.<xmlattr>.name" ) )
    {
        tree_.put( "type", GetType() );
        tree_.put( "name", name_ );
        tree_.put( "date", GetDate( file ) );
        MaybeCopy( tree_, "name", more, "exercise.meta.name" );
        MaybeCopy( tree_, "briefing", more, "exercise.meta.briefing.text" );
        MaybeCopy( tree_, "model",   more, "exercise.model.<xmlattr>.dataset" );
        MaybeCopy( tree_, "terrain", more, "exercise.terrain.<xmlattr>.name" );
    }

    std::string GetType() const
    {
        return "exercise";
    }

    Path GetSuffix() const
    {
        return Path( "exercises" ) / name_;
    }

    std::string GetDependency( const Tree& tree, const std::string& key )
    {
        const boost::optional< std::string > data = tree.get_optional< std::string >( key );
        return data == boost::none ? "" : *data;
    }

    std::vector< boost::shared_ptr< Item > > GetDependencies() const
    {
        std::vector< boost::shared_ptr< Item > > deps;
        if( !model_.empty() )
            deps.push_back( boost::make_shared< Dependency >( system_, "model", model_ ) );
        if( !terrain_.empty() )
            deps.push_back( boost::make_shared< Dependency >( system_, "terrain", terrain_ ) );
        return deps;
    }

    template< typename T >
    static void Parse( const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const boost::optional< std::string >& version )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "exercises", L"exercise.xml" ) )
        {
            const Tree more = FromXml( system.ReadFile( path ) );
            items.push_back( boost::make_shared< Exercise >( system, path, ++idx, version, more ) );
        }
    }

    const FileSystem_ABC& system_;
    const std::string model_;
    const std::string terrain_;
};
}

// -----------------------------------------------------------------------------
// Name: Package::Package
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Package::Package( const FileSystem_ABC& system, const Path& path, bool reference )
    : system_   ( system )
    , path_     ( path )
    , reference_( reference )
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
Tree Package::GetProperties() const
{
    Tree tree;
    if( name_.empty() )
        return tree;
    tree.put( "name", name_ );
    tree.put( "description", description_ );
    if( !version_.empty() )
        tree.put( "version", version_ );
    tree.put_child( "items", Tree() );
    Tree& items = tree.get_child( "items" );
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        items.push_back( std::make_pair( "", item->GetProperties() ) );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Package::Parse
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
bool Package::Parse()
{
    boost::optional< std::string > version;
    if( !reference_ )
    {
        const Path index = path_ / "content.xml";
        if( !system_.IsFile( index ) )
            return false;

        Tree tree;
        try
        {
            tree = FromXml( system_.ReadFile( index ) );
        }
        catch( const std::exception& /*err*/ )
        {
            return false;
        }

        std::string name, description;
        MaybeGet( name, tree, "content.name" );
        MaybeGet( description, tree, "content.description" );
        if( name.empty() || description.empty() )
            return false;

        name_ = name;
        description_ = description;
        version = tree.get_optional< std::string >( "content.version" );
        version_ = version == boost::none ? "Unversioned" : *version;
    }

    items_.clear();
    size_t idx = 0;
    Model::Parse( system_, path_, items_, idx, version );
    Terrain::Parse( system_, path_, items_, idx, version );
    Exercise::Parse( system_, path_, items_, idx, version );
    BOOST_FOREACH( const T_Items::value_type& value, items_ )
        value->Join();

    return true;
}

// -----------------------------------------------------------------------------
// Name: Package::Identify
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
void Package::Identify( const Package_ABC& ref )
{
    BOOST_FOREACH( const T_Items::value_type& value, items_ )
        value->Identify( ref, *this );
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Package_ABC::Item_ABC* Package::Find( size_t id ) const
{
    BOOST_FOREACH( const T_Items::value_type& value, items_ )
        if( value->Compare( id ) )
            return value.get();
    return NULL;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Package_ABC::Item_ABC* Package::Find( const Item_ABC& item ) const
{
    BOOST_FOREACH( const T_Items::value_type& value, items_ )
        if( value->Compare( item ) )
            return value.get();
    return NULL;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Package::Install( const Package_ABC& src, const std::vector< size_t >& ids )
{
    std::vector< Item_ABC* > targets;
    BOOST_FOREACH( size_t id, ids )
    {
        Item_ABC* item = src.Find( id );
        if( item )
            targets.push_back( item );
    }
    std::vector< TaskHandler< void >::Future > futures;
    BOOST_FOREACH( Item_ABC* item, targets )
        futures.push_back( TaskHandler< void >::Go(
            boost::bind( &Item_ABC::Install, item,
                boost::cref( system_ ), boost::cref( path_ ), boost::cref( *this ), boost::cref( targets ) ) ) );
    boost::wait_for_all( futures.begin(), futures.end() );
}