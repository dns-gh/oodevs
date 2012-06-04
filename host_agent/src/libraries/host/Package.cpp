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
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace host;
using runtime::Utf8Convert;

struct Package_ABC::Item_ABC : public boost::noncopyable
{
    virtual Tree        GetProperties() const = 0;
    virtual std::string GetType() const = 0;
    virtual Path        GetName() const = 0;
    virtual Path        GetRoot() const = 0;
    virtual Path        GetSuffix() const = 0;
    virtual std::string GetChecksum() const = 0;
    virtual bool        Compare( size_t id ) const = 0;
    virtual bool        Compare( const Item_ABC& item ) const = 0;
    virtual bool        IsExercise() const = 0;
    virtual void        Identify( const Package_ABC& ref, const Package_ABC& root ) = 0;
    virtual void        Install( const FileSystem_ABC& system, const Path& trash, const Path& output, const Package_ABC& dst, const Package::T_Items& targets ) const = 0;
    virtual void        Move( const FileSystem_ABC& system, const Path& dst ) const = 0;
    virtual void        SetExercisePaths( const Package_ABC& src, Path& model, Path& terrain, Path& exercise ) const = 0;
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

    Metadata()
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
        return Get( tree, key, "Unversioned" );
    }

    static Metadata Reload( const FileSystem_ABC& system, const Path& root )
    {
        return Metadata( FromJson( system.ReadFile( root / GetFilename() ) ) );
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

    Metadata& operator=( const Metadata& );

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

int GetTypeOrder( const std::string& type )
{
    if( type == "model" )    return 0;
    if( type == "terrain" )  return 1;
    if( type == "exercise" ) return 2;
    return 3;
}

template< typename T >
int Compare( const T& a, const T& b )
{
    return a == b ? 0 : a < b ? -1 : +1;
}

bool ItemOrder( const Package_ABC::T_Item& lhs, const Package_ABC::T_Item& rhs )
{
    int rpy = Compare( GetTypeOrder( lhs->GetType() ), GetTypeOrder( rhs->GetType() ) );
    if( rpy )
        return rpy < 0;
    rpy = Compare( lhs->GetName(), rhs->GetName() );
    if( rpy )
        return rpy < 0;
    return lhs->GetChecksum() < rhs->GetChecksum();
}

struct Item : Package_ABC::Item_ABC
{
    Item( const FileSystem_ABC& system, const Path& root, size_t id, const std::string& name, const std::string& date, const Metadata* meta )
        : root_ ( root )
        , id_   ( id )
        , name_ ( name )
        , date_ ( date )
        , meta_ ( meta ? *meta : Metadata::Reload( system, root ) )
    {
        // NOTHING
    }

    Item( const std::string& name )
        : id_   ( 0 )
        , name_ ( name )
    {
        // NOTHING
    }

    virtual ~Item()
    {
        // NOTHING
    }

    virtual Tree GetProperties() const
    {
        Tree tree;
        meta_.SaveTo( tree );
        tree.put( "id", id_ );
        tree.put( "type", GetType() );
        tree.put( "name", Utf8Convert( name_ ) );
        tree.put( "date", date_ );
        tree.put( "checksum", checksum_ );
        if( !action_.empty() )
            tree.put( "action", action_ );
        if( !error_.empty() )
            tree.put( "error", error_ );
        return tree;
    }

    virtual Path GetRoot() const
    {
        return root_;
    }

    virtual Path GetName() const
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

    typedef std::vector< Package_ABC::T_Item > T_Dependencies;
    virtual T_Dependencies GetDependencies() const
    {
        return T_Dependencies();
    }

    virtual void Identify( const Package_ABC& ref, const Package_ABC& root )
    {
        action_.clear();
        error_.clear();
        BOOST_FOREACH( const T_Dependencies::value_type& dep, GetDependencies() )
            if( !ref.Find( *dep ) && !root.Find( *dep ) )
            {
                action_ = "error";
                error_ = "Missing " + dep->GetType() + " " + Utf8Convert( dep->GetName() );
                return;
            }
        Package_ABC::T_Item next = ref.Find( *this );
        if( !next )
            action_ = "add";
        else if( checksum_ != next->GetChecksum() )
            action_ = "update";
    }

    void MakeChecksum( const FileSystem_ABC& system )
    {
        checksum_ = system.Checksum( root_ / GetSuffix() );
    }

    void Install( const FileSystem_ABC& system, const Path& trash, const Path& output, const Package_ABC& dst, const Package::T_Items& targets ) const
    {
        BOOST_FOREACH( const T_Dependencies::value_type& dep, GetDependencies() )
            if( !dst.Find( *dep ) && !HasItem( targets, *dep ) )
                return;
        Package_ABC::T_Item old = dst.Find( *this );
        const Path next = output / GetSuffix();
        system.MakePaths( next );
        system.CopyDirectory( root_ / GetSuffix(), next );
        meta_.Save( system, output );
        if( old )
            old->Move( system, trash );
    }

    void Move( const FileSystem_ABC& system, const Path& dst ) const
    {
        system.Rename( root_, dst );
    }

    bool IsExercise() const
    {
        return false;
    }

    void SetExercisePaths( const Package_ABC&, Path&, Path&, Path& ) const
    {
        throw std::runtime_error( "Unexpected call to SetExercisePaths on non-exercise item" );
    }

protected:
    const Path root_;
    const size_t id_;
    const Path name_;
    const std::string date_;
    const Metadata meta_;
    std::string checksum_;
    std::string action_;
    std::string error_;
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

template< typename T, typename U >
void AttachItem( Async& async, const FileSystem_ABC& system, T& items, const U& item )
{
    items.push_back( item );
    async.Go( boost::bind( &Item::MakeChecksum, item, boost::cref( system ) ) );
}

struct Model : public Item
{
    Model( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta )
        : Item( system, root, id, Utf8Convert( Path( file ).remove_filename().remove_filename().filename() ), GetDate( file ), meta )
    {
        // NOTHING
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
    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, T& items, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "models", L"decisional.xml" ) )
            AttachItem( async, system, items, boost::make_shared< Model >( system, root, path, items.size(), meta ) );
    }
};

struct Terrain : public Item
{
    Terrain( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta )
        : Item( system, root, id, GetFilename( file, "terrains" ), GetDate( file ), meta )
    {
        // NOTHING
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
    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, T& items, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "data" / "terrains", L"Terrain.xml" ) )
            AttachItem( async, system, items,
                        boost::make_shared< Terrain >( system, root, path, items.size(), meta ) );
    }
};

struct Dependency : public Item
{
    Dependency( const std::string& type, const std::string& name )
        : Item ( name )
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

void SetPathFromDependency( Path& dst, const Package_ABC& src, const std::string& type, const std::string& name )
{
    Package_ABC::T_Item item = src.Find( Dependency( type, name ) );
    if( item )
        dst = item->GetRoot();
}

struct Exercise : public Item
{
    Exercise( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta, const Tree& more )
        : Item     ( system, root, id, GetFilename( file, "exercises" ), GetDate( file ), meta )
        , briefing_( Get( more, "exercise.meta.briefing.text" ) )
        , model_   ( Get( more, "exercise.model.<xmlattr>.dataset" ) )
        , terrain_ ( Get( more, "exercise.terrain.<xmlattr>.name" ) )
    {
        // NOTHING
    }

    virtual Tree GetProperties() const
    {
        Tree tree = Item::GetProperties();
        tree.put( "briefing", briefing_ );
        tree.put( "model", model_ );
        tree.put( "terrain", terrain_ );
        return tree;
    }

    std::string GetType() const
    {
        return "exercise";
    }

    Path GetSuffix() const
    {
        return Path( "exercises" ) / name_;
    }

    T_Dependencies GetDependencies() const
    {
        T_Dependencies deps;
        if( !model_.empty() )
            deps.push_back( boost::make_shared< Dependency >( "model", model_ ) );
        if( !terrain_.empty() )
            deps.push_back( boost::make_shared< Dependency >( "terrain", terrain_ ) );
        return deps;
    }

    bool IsExercise() const
    {
        return true;
    }

    void SetExercisePaths( const Package_ABC& src, Path& model, Path& terrain, Path& exercise ) const
    {
        exercise = root_;
        SetPathFromDependency( model, src, "model", model_ );
        SetPathFromDependency( terrain, src, "terrain", terrain_ );
    }

    template< typename T >
    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, T& items, const Metadata* meta )
    {
        BOOST_FOREACH( const Path& path, system.Glob( root / "exercises", L"exercise.xml" ) )
            AttachItem( async, system, items,
                        boost::make_shared< Exercise >( system, root, path, items.size(), meta, FromXml( system.ReadFile( path ) ) ) );
    }

    const std::string briefing_;
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
    Tree& items = tree.put_child( "items", Tree() );
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        items.push_back( std::make_pair( "", item->GetProperties() ) );
    return tree;
}

// -----------------------------------------------------------------------------
// Name: Package::GetPath
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
Path Package::GetPath() const
{
    return path_;
}

namespace
{
template< typename T >
void FillItems( Async& async, const FileSystem_ABC& system, const Path& path, T& items, Metadata* meta )
{
    Model::Parse   ( async, system, path, items, meta );
    Terrain::Parse ( async, system, path, items, meta );
    Exercise::Parse( async, system, path, items, meta );
}
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

        const Tree tree = FromXml( system_.ReadFile( index ) );
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
    Async async( pool_ );
    if( reference_ )
        BOOST_FOREACH( const Path& path, system_.Walk( path_, false ) )
            FillItems( async, system_, path, items_, meta.get() );
    else
        FillItems( async, system_, path_, items_, meta.get() );
    async.Join();
    std::sort( items_.begin(), items_.end(), &ItemOrder );
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
// Name: Package::Install
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Package::Install( const Path& trash, const Package_ABC& src, const std::vector< size_t >& ids )
{
    T_Items install;
    BOOST_FOREACH( size_t id, ids )
    {
        T_Item item = src.Find( id );
        if( item )
            install.push_back( item );
    }
    if( install.empty() )
        return;
    system_.MakePaths( path_ );
    Async async( pool_ );
    size_t idx = 0;
    BOOST_FOREACH( const T_Items::value_type& item, install )
        async.Go( boost::bind( &Item_ABC::Install, item, boost::cref( system_ ),
                  trash / boost::lexical_cast< std::string >( idx++ ),
                  system_.MakeAnyPath( path_ ), boost::cref( *this ), boost::cref( install ) ) );
}

namespace
{
bool IsItemIn( const std::vector< size_t >& list, const Package_ABC::T_Item& item )
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
void Package::Move( const Path& trash, const std::vector< size_t >& ids )
{
    Async async( pool_ );
    size_t idx = 0;
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        if( IsItemIn( ids, item ) )
            async.Go( boost::bind( &Item_ABC::Move, item, boost::cref( system_ ),
                      trash / boost::lexical_cast< std::string >( idx++ ) ) );
    async.Join();
    items_.erase( std::remove_if( items_.begin(), items_.end(), boost::bind( &IsItemIn, boost::cref( ids ), _1 ) ), items_.end() );
}

// -----------------------------------------------------------------------------
// Name: Package::GetExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
Package::T_Exercises Package::GetExercises( int offset, int limit ) const
{
    T_Exercises reply;
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
    {
        if( !item->IsExercise() )
            continue;
        if( offset-- > 0 )
            continue;
        reply.push_back( Utf8Convert( item->GetName() ) );
        if( --limit <  0 )
            break;
    }
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Package::CountExercises
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
size_t Package::CountExercises() const
{
    size_t reply = 0;
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        reply += item->IsExercise();
    return reply;
}

// -----------------------------------------------------------------------------
// Name: Package::SetExercisePaths
// Created: BAX 2012-06-04
// -----------------------------------------------------------------------------
void Package::SetExercisePaths( const std::string& name, Path& model, Path& terrain, Path& exercise ) const
{
    Package_ABC::T_Item next = Find( Dependency( "exercise", name ) );
    if( next )
        next->SetExercisePaths( *this, model, terrain, exercise );
}