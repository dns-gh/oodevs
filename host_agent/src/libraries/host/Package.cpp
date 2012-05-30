// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Package.h"

#include "Async.h"
#include "FileSystem_ABC.h"
#include "PropertyTree.h"
#include "runtime/Utf8.h"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
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
    virtual void        Install( const FileSystem_ABC& system, const Path& output, const Package_ABC& dst, const Package::T_Items& targets ) const = 0;
    virtual void        Move( const FileSystem_ABC& system, const Path& dst ) const = 0;
    virtual void        Displace( const Path& path ) = 0;
};

namespace
{
std::string Get( const Tree& tree, const std::string& key, const std::string& def = std::string() )
{
    const boost::optional< std::string > data = tree.get_optional< std::string >( key );
    return data == boost::none ? def : *data;
}

struct Metadata
{
    Metadata( const std::string& package, const std::string& version )
        : package_( package )
        , version_( version )
    {
        // NOTHING
    }

    std::string GetVersion() const
    {
        return version_;
    }

    Tree GetProperties() const
    {
        Tree rpy;
        SaveTo( rpy );
        return rpy;
    }

    void SaveTo( Tree& tree ) const
    {
        tree.put( "package", package_ );
        tree.put( "version", version_ );
    }

    static std::string GetFilename()
    {
        return "metadata.tag";
    }

    static std::string GetVersion( const Tree& tree, const std::string& key )
    {
        return Get( tree, "version", "Unversioned" );
    }

    static Metadata Reload( const FileSystem_ABC& system, const Path& root )
    {
        Tree tree;
        try
        {
            const std::string data = system.ReadFile( root / GetFilename() );
            if( !data.empty() )
                tree = FromJson( data );
        }
        catch( const std::exception& /*err*/ )
        {
        }
        return Metadata( tree );
    }

    void Save( const FileSystem_ABC& system, const Path& root ) const
    {
        system.WriteFile( root / GetFilename(), ToJson( GetProperties(), true ) );
    }

private:
    Metadata( const Tree& tree )
        : package_( Get( tree, "package", "" ) )
        , version_( GetVersion( tree, "version" ) )
    {
        // NOTHING
    }

    const std::string package_;
    const std::string version_;
};

template< typename T, typename U >
bool ItemCompare( const T& lhs, const U& rhs )
{
    return lhs->Compare( rhs );
}

template< typename T, typename U >
typename T::const_iterator FindItem( const T& list, const U& item )
{
    return std::find_if( list.begin(), list.end(), boost::bind( &ItemCompare< typename T::value_type, U >, _1, boost::cref( item ) ) );
}

template< typename T, typename U >
bool HasItem( const T& list, const U& item )
{
    return FindItem( list, item ) != list.end();
}

struct Item : Package_ABC::Item_ABC
{
    Item( Pool_ABC& pool, const FileSystem_ABC& system, const Path& root, size_t id, const std::string& name, const Metadata* meta )
        : root_ ( root )
        , id_   ( id )
        , name_ ( name )
        , meta_ ( meta ? *meta : Metadata::Reload( system, root_ ) )
        , async_( pool )
    {
        tree_.put( "id", id_ );
        meta_.SaveTo( tree_ );
        if( !root_.empty() )
            async_.Go( boost::bind( &Item::MakeChecksum, this, boost::cref( system ) ) );
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
        tree_.erase( "action" );
        tree_.erase( "error" );
        BOOST_FOREACH( const boost::shared_ptr< Item >& dep, GetDependencies() )
            if( !ref.Find( *dep ) && !root.Find( *dep ) )
            {
                tree_.put( "action", "error" );
                tree_.put( "error", "Missing " + dep->GetType() + " " + dep->name_ );
                return;
            }
        Package_ABC::T_Item next = ref.Find( *this );
        if( !next )
            tree_.put( "action", "add" );
        else if( checksum_ != next->GetChecksum() )
            tree_.put( "action", "update" );
    }

    void MakeChecksum( const FileSystem_ABC& system )
    {
        checksum_ = system.Checksum( root_, Metadata::GetFilename() );
    }

    void Join()
    {
        async_.Join();
        tree_.put( "checksum", checksum_ );
    }

    void Install( const FileSystem_ABC& system, const Path& output, const Package_ABC& dst, const Package::T_Items& targets ) const
    {
        BOOST_FOREACH( const boost::shared_ptr< Item >& dep, GetDependencies() )
            if( !dst.Find( *dep ) && !HasItem( targets, *dep ) )
                return;
        const Path& next = output / GetSuffix();
        system.Remove( next );
        system.MakeDirectory( next );
        system.CopyDirectory( root_, next );
        meta_.Save( system, next );
    }

    void Move( const FileSystem_ABC& system, const Path& dst ) const
    {
        system.Rename( root_, dst );
    }

    void Displace( const Path& path )
    {
        root_ = path / GetSuffix();
    }

protected:
    const size_t id_;
    const std::string name_;
    const Metadata meta_;
    Path root_;
    Tree tree_;
    std::string checksum_;
    Async async_;
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
    Model( Pool_ABC& pool, const FileSystem_ABC& system, const Path& file, size_t id, const Metadata* meta )
        : Item( pool, system, Path( file ).remove_filename().remove_filename(), id,
                Utf8Convert( Path( file ).remove_filename().remove_filename().filename() ), meta )
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
    static void Parse( Pool_ABC& pool, const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "models", L"decisional.xml" ) )
            items.push_back( boost::make_shared< Model >( pool, system, path, ++idx, meta ) );
    }
};

struct Terrain : public Item
{
    Terrain( Pool_ABC& pool, const FileSystem_ABC& system, const Path& file, size_t id, const Metadata* meta )
        : Item ( pool, system, Path( file ).remove_filename(), id, GetFilename( file, "terrains" ), meta )
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
    static void Parse( Pool_ABC& pool, const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "terrains", L"Terrain.xml" ) )
            items.push_back( boost::make_shared< Terrain >( pool, system, path, ++idx, meta ) );
    }
};

struct Dependency : public Item
{
    Dependency( Pool_ABC& pool, const FileSystem_ABC& system, const std::string& type, const std::string& name )
        : Item ( pool, system, "", 0, name, 0 )
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
    Exercise( Pool_ABC& pool, const FileSystem_ABC& system, const Path& file, size_t id, const Metadata* meta, const Tree& more )
        : Item    ( pool, system, Path( file ).remove_filename(), id, GetFilename( file, "exercises" ), meta )
        , pool_   ( pool )
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
            deps.push_back( boost::make_shared< Dependency >( pool_, system_, "model", model_ ) );
        if( !terrain_.empty() )
            deps.push_back( boost::make_shared< Dependency >( pool_, system_, "terrain", terrain_ ) );
        return deps;
    }

    template< typename T >
    static void Parse( Pool_ABC& pool, const FileSystem_ABC& system, const Path& root, T& items, size_t& idx, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "exercises", L"exercise.xml" ) )
        {
            const Tree more = FromXml( system.ReadFile( path ) );
            items.push_back( boost::make_shared< Exercise >( pool, system, path, ++idx, meta, more ) );
        }
    }

    Pool_ABC& pool_;
    const FileSystem_ABC& system_;
    const std::string model_;
    const std::string terrain_;
};
}

// -----------------------------------------------------------------------------
// Name: Package::Package
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Package::Package( Pool_ABC& pool, const FileSystem_ABC& system, const Path& path, bool reference )
    : pool_     ( pool )
    , system_   ( system )
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
    if( !reference_ )
    {
        if( name_.empty() )
            return tree;
        tree.put( "name", name_ );
        tree.put( "description", description_ );
        if( !version_.empty() )
            tree.put( "version", version_ );
    }
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
    std::auto_ptr< Metadata > meta;

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

        const std::string name = Get( tree, "content.name" );
        const std::string description = Get( tree, "content.description" );
        if( name.empty() || description.empty() )
            return false;

        name_ = name;
        description_ = description;
        version_ = Metadata::GetVersion( tree, "content.version" );
        meta.reset( new Metadata( name_, version_ ) );
    }

    items_.clear();
    size_t idx = 0;
    Model::Parse( pool_, system_, path_, items_, idx, meta.get() );
    Terrain::Parse( pool_, system_, path_, items_, idx, meta.get() );
    Exercise::Parse( pool_, system_, path_, items_, idx, meta.get() );
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
Package_ABC::T_Item Package::Find( size_t id ) const
{
    T_Items::const_iterator it = FindItem( items_, id );
    return it == items_.end() ? T_Item() : *it;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Package_ABC::T_Item Package::Find( const Item_ABC& item ) const
{
    T_Items::const_iterator it = FindItem( items_, item );
    return it == items_.end() ? T_Item() : *it;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Package::Install( const Package_ABC& src, const std::vector< size_t >& ids )
{
    T_Items targets;
    BOOST_FOREACH( size_t id, ids )
    {
        T_Item item = src.Find( id );
        if( item )
            targets.push_back( item );
    }
    Async async( pool_ );
    BOOST_FOREACH( const T_Items::value_type& item, targets )
        async.Go( boost::bind( &Item_ABC::Install, item, boost::cref( system_ ), boost::cref( path_ ), boost::cref( *this ), boost::cref( targets ) ) );
}

namespace
{
bool IsItemIn( const std::vector< size_t >& list, const boost::shared_ptr< Package_ABC::Item_ABC >& item )
{
    BOOST_FOREACH( const size_t& id, list )
        if( item->Compare( id ) )
            return true;
    return false;
}
}

// -----------------------------------------------------------------------------
// Name: Package::Move
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
void Package::Move( const Path& dst, const std::vector< size_t >& ids )
{
    Async async( pool_ );
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        if( IsItemIn( ids, item ) )
            async.Go( boost::bind( &Item_ABC::Move, item, boost::cref( system_ ), dst ) );
    async.Join();
    items_.erase( std::remove_if( items_.begin(), items_.end(), boost::bind( &IsItemIn, boost::cref( ids ), _1 ) ), items_.end() );
}

// -----------------------------------------------------------------------------
// Name: Package::Move
// Created: BAX 2012-05-29
// -----------------------------------------------------------------------------
void Package::Move( const Path& path )
{
    system_.Rename( path_, path );
    path_ = path;
    BOOST_FOREACH( const T_Items::value_type& it, items_ )
        it->Displace( path );
}