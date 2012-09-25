// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Package.h"

#include "runtime/Async.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Chunker_ABC.h"
#include "web/HttpException.h"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace host;
using namespace property_tree;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;
using runtime::Utf8;

typedef boost::function< void( const Path& ) > PathOperand;

struct Package_ABC::Item_ABC : public boost::noncopyable
{
    virtual Tree        GetProperties() const = 0;
    virtual size_t      GetSize() const = 0;
    virtual std::string GetType() const = 0;
    virtual Path        GetName() const = 0;
    virtual Path        GetRoot() const = 0;
    virtual Path        GetSuffix() const = 0;
    virtual std::string GetChecksum() const = 0;
    virtual bool        Compare( size_t id ) const = 0;
    virtual bool        Compare( const Item_ABC& item ) const = 0;
    virtual bool        Compare( const std::pair< Path, std::string >& pair ) const = 0;
    virtual bool        IsExercise() const = 0;
    virtual bool        IsInstalled() const = 0;
    virtual bool        IsLinked() const = 0;
    virtual bool        IsValid() const = 0;
    virtual void        Identify( const Package_ABC& ref, const Package_ABC& root ) = 0;
    virtual void        Install( Async& async, const FileSystem_ABC& system, const Path& tomb, const Path& output,
                                 const Package_ABC& dst, const Package::T_Items& targets, const PathOperand& operand ) const = 0;
    virtual void        Uninstall( Async& async, const FileSystem_ABC& system, const Path& dst ) = 0;
    virtual void        Reinstall( const FileSystem_ABC& system ) = 0;
    virtual void        Link( Tree& tree, const Package_ABC& ref, bool recurse ) = 0;
    virtual bool        Unlink( Async& async, const FileSystem_ABC& system ) = 0;
    virtual void        Download( const FileSystem_ABC& fs, web::Chunker_ABC& dst ) const = 0;
};

namespace
{
struct Metadata
{
    Metadata( const std::string& package, const std::string& version )
        : package_( package )
        , version_( version )
        , links_  ( 0 )
    {
        // NOTHING
    }

    Metadata()
        : links_( 0 )
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
        if( !tomb_.empty() )
            rpy.put( "tomb", Utf8( tomb_ ) );
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
        return Get< std::string >( tree, key, "Unversioned" );
    }

    static Metadata Reload( const FileSystem_ABC& system, const Path& root )
    {
        return Metadata( FromJson( system.ReadFile( root / GetFilename() ) ) );
    }

    void Save( const FileSystem_ABC& system, const Path& root ) const
    {
        system.WriteFile( root / GetFilename(), ToJson( GetProperties(), true ) );
    }

    void TryUninstall( Async& async, const FileSystem_ABC& system, const Path& root, const Path& tomb )
    {
        tomb_ = tomb;
        if( IsLinked() )
            Save( system, root );
        TryKill( async, system, root );
    }

    void Reinstall( const FileSystem_ABC& system, const Path& root )
    {
        tomb_.clear();
        Save( system, root );
    }

    bool IsInstalled() const
    {
        return tomb_.empty();
    }

    bool IsLinked() const
    {
        return !!links_;
    }

    void Link()
    {
        links_++;
    }

    bool Unlink( Async& async, const FileSystem_ABC& system, const Path& root )
    {
        links_--;
        return TryKill( async, system, root );
    }

private:
    Metadata( const Tree& tree )
        : package_( Get< std::string >( tree, "package", "" ) )
        , version_( GetVersion( tree, "version" ) )
        , tomb_   ( Utf8( Get< std::string >( tree, "tomb", "" ) ) )
        , links_  ( 0 )
    {
        // NOTHING
    }

    Metadata& operator=( const Metadata& );

    bool TryKill( Async& async, const FileSystem_ABC& system, const Path& root )
    {
        if( IsLinked() || IsInstalled() )
            return false;
        const Path next = system.MakeAnyPath( tomb_ );
        system.Rename( root, next / "_" );
        async.Go( boost::bind( &FileSystem_ABC::Remove, &system, next ) );
        return true;
    }

    const std::string package_;
    const std::string version_;
    Path tomb_;
    size_t links_;
};

template< typename T, typename U >
bool ItemCompare( const T& lhs, const U& rhs, bool installed )
{
    return lhs->Compare( rhs ) && ( !installed || lhs->IsInstalled() );
}

template< typename T, typename U >
typename T::const_iterator FindItem( const T& list, const U& item, bool installed )
{
    return std::find_if( list.begin(), list.end(), boost::bind( &ItemCompare< typename T::value_type, U >, _1, boost::cref( item ), installed ) );
}

template< typename T, typename U >
bool HasItem( const T& list, const U& item )
{
    return FindItem( list, item, true ) != list.end();
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

bool BeginWith( const Path& prefix, const Path& path )
{
    for( Path::const_iterator a = prefix.begin(), b = path.begin(); a != prefix.end(); ++a, ++b )
        if( b == path.end() )
            return false;
        else if( *a != *b )
            return false;
    return true;
}

struct Item : Package_ABC::Item_ABC
{
    Item( const FileSystem_ABC& system, const Path& root, size_t id, const std::string& name, const std::string& date, const Metadata* meta )
        : root_ ( root )
        , id_   ( id )
        , name_ ( name )
        , date_ ( date )
        , meta_ ( meta ? *meta : Metadata::Reload( system, root ) )
        , size_ ( 0 )
    {
        // NOTHING
    }

    Item( const std::string& name )
        : id_   ( 0 )
        , name_ ( name )
        , size_ ( 0 )
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
        tree.put( "id", id_ );
        tree.put( "type", GetType() );
        tree.put( "name", Utf8( name_ ) );
        tree.put( "date", date_ );
        tree.put( "checksum", checksum_ );
        tree.put( "size", size_ );
        meta_.SaveTo( tree );
        if( !action_.empty() )
            tree.put( "action", action_ );
        if( !error_.empty() )
            tree.put( "error", error_ );
        return tree;
    }

    virtual size_t GetSize() const
    {
        return size_;
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

    virtual bool Compare( const std::pair< Path, std::string >& pair ) const
    {
        return pair.first == root_ && pair.second == checksum_;
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
            if( !ref.Find( *dep, true ) && !root.Find( *dep, true ) )
            {
                action_ = "error";
                error_ = "Missing " + dep->GetType() + " " + Utf8( dep->GetName() );
                return;
            }
        Package_ABC::T_Item next = ref.Find( *this, true );
        if( !next )
            action_ = "add";
        else if( checksum_ != next->GetChecksum() )
            action_ = "update";
    }

    FileSystem_ABC::T_Predicate IsItemFile( const Path& root ) const
    {
        if( !IsExercise() )
            return FileSystem_ABC::T_Predicate();
        return !boost::bind( &BeginWith, root / "sessions", _1 );
    }

    void MakeChecksum( const FileSystem_ABC& system )
    {
        size_t read;
        const Path root = root_ / GetSuffix();
        checksum_ = system.Checksum( root, IsItemFile( root ), read );
        size_ = read;
    }

    void Install( Async& async, const FileSystem_ABC& system, const Path& tomb, const Path& root, const Package_ABC& dst, const Package::T_Items& targets, const PathOperand& operand ) const
    {
        BOOST_FOREACH( const T_Dependencies::value_type& dep, GetDependencies() )
            if( !dst.Find( *dep, true ) && !HasItem( targets, *dep ) )
                return;

        Package_ABC::T_Item old = dst.Find( *this, false );
        if( old && old->GetChecksum() == checksum_ )
            return old->Reinstall( system );

        const Path output = system.MakeAnyPath( root );
        const Path sub =  output / GetSuffix();
        system.MakePaths( sub );
        system.CopyDirectory( root_ / GetSuffix(), sub );
        meta_.Save( system, output );
        if( old )
            old->Uninstall( async, system, tomb );
        operand( output );
    }

    void Uninstall( Async& async, const FileSystem_ABC& system, const Path& dst )
    {
        meta_.TryUninstall( async, system, root_, dst );
    }

    void Reinstall( const FileSystem_ABC& system )
    {
        meta_.Reinstall( system, root_ );
    }

    bool IsExercise() const
    {
        return false;
    }

    bool IsInstalled() const
    {
        return meta_.IsInstalled();
    }

    bool IsLinked() const
    {
        return meta_.IsLinked();
    }

    bool IsValid() const
    {
        return error_.empty();
    }

    void Link( Tree& tree, const Package_ABC& ref, bool recurse )
    {
        const std::string prefix = GetType();
        tree.put( prefix + ".name", Utf8( name_ ) );
        tree.put( prefix + ".root", Utf8( root_ ) );
        tree.put( prefix + ".checksum", checksum_ );
        if( recurse )
            BOOST_FOREACH( const T_Dependencies::value_type& it, GetDependencies() )
                for( Package_ABC::T_Item next = ref.Find( *it, true ); next; next.reset() )
                    next->Link( tree, ref, false );
        meta_.Link();
    }

    bool Unlink( Async& async, const FileSystem_ABC& system )
    {
        return meta_.Unlink( async, system, root_ );
    }

    void Download( const FileSystem_ABC& fs, web::Chunker_ABC& dst ) const
    {
        dst.SetName( Utf8( name_ ) );
        dst.SetHeader( "Original-Content-Length", boost::lexical_cast< std::string >( size_ ) );
        io::Writer_ABC& io = dst.OpenWriter();
        FileSystem_ABC::T_Packer packer = fs.Pack( io );
        packer->Pack( root_, IsItemFile( GetSuffix() ) );
    }

protected:
    const Path root_;
    const size_t id_;
    const Path name_;
    const std::string date_;
    Metadata meta_;
    std::string checksum_;
    std::string action_;
    std::string error_;
    size_t size_;
};

std::string Format( const std::time_t& time )
{
    char tmp[64];
    const size_t size = strftime( tmp, sizeof tmp, "%Y-%m-%d %H:%M:%S", localtime( &time ) );
    return std::string( tmp, size );
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
    return Utf8( reply );
}

template< typename T >
void AttachItem( Async& async, const FileSystem_ABC& system, Package::T_Items& items, const T& item )
{
    items.push_back( item );
    async.Go( boost::bind( &Item::MakeChecksum, item, boost::cref( system ) ) );
}

template< typename T >
bool AttachSimple( Async& async, const Path& path, const FileSystem_ABC& system, const Path& root, Package::T_Items& items, const Metadata* meta )
{
    AttachItem( async, system, items, boost::make_shared< T >( system, root, path, items.size(), meta ) );
    return true;
}

template< typename T >
bool AttachExtended( Async& async, const Path& path, const FileSystem_ABC& system, const Path& root, Package::T_Items& items, const Metadata* meta )
{
    AttachItem( async, system, items, boost::make_shared< T >( system, root, path, items.size(), meta, FromXml( system.ReadFile( path ) ) ) );
    return true;
}

struct Model : public Item
{
    Model( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta )
        : Item( system, root, id, Utf8( Path( file ).remove_filename().remove_filename().filename() ), Format( system.GetLastWrite( file ) ), meta )
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

    static bool Filter( const FileSystem_ABC& system, const Path& path, FileSystem_ABC::T_Predicate operand, const std::string& suffix )
    {
        const Path target = path / suffix;
        if( system.IsFile( target ) )
            operand( target );
        return true;
    }

    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, Package::T_Items& items, const Metadata* meta )
    {
        FileSystem_ABC::T_Predicate op = boost::bind( AttachSimple< Model >, boost::ref( async ), _1, boost::cref( system ), boost::cref( root ), boost::ref( items ), meta );
        system.Walk( root / "data" / "models", false, boost::bind( &Model::Filter, boost::cref( system ), _1, op, "decisional/decisional.xml" ) );
    }
};

struct Terrain : public Item
{
    Terrain( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta )
        : Item( system, root, id, GetFilename( file, "terrains" ), Format( system.GetLastWrite( file ) ), meta )
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

    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, Package::T_Items& items, const Metadata* meta )
    {
        FileSystem_ABC::T_Predicate op = boost::bind( AttachSimple< Terrain >, boost::ref( async ), _1, boost::cref( system ), boost::cref( root ), boost::ref( items ), meta );
        system.Glob( root / "data" / "terrains", "Terrain.xml", op );
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

struct Exercise : public Item
{
    Exercise( const FileSystem_ABC& system, const Path& root, const Path& file, size_t id, const Metadata* meta, const Tree& more )
        : Item     ( system, root, id, GetFilename( file, "exercises" ), Format( system.GetLastWrite( file ) ), meta )
        , briefing_( Get< std::string >( more, "exercise.meta.briefing.text" ) )
        , model_   ( Get< std::string >( more, "exercise.model.<xmlattr>.dataset" ) )
        , terrain_ ( Get< std::string >( more, "exercise.terrain.<xmlattr>.name" ) )
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

    static void Parse( Async& async, const FileSystem_ABC& system, const Path& root, Package::T_Items& items, const Metadata* meta )
    {
        FileSystem_ABC::T_Predicate op = boost::bind( AttachExtended< Exercise >, boost::ref( async ), _1, boost::cref( system ), boost::cref( root ), boost::ref( items ), meta );
        system.Glob( root / "exercises", "exercise.xml", op );
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
        if( item->IsInstalled() )
            items.push_back( std::make_pair( std::string(), item->GetProperties() ) );
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
bool FillItems( Async& async, const FileSystem_ABC& system, const Path& path, Package::T_Items& items, Metadata* meta )
{
    Model::Parse   ( async, system, path, items, meta );
    Terrain::Parse ( async, system, path, items, meta );
    Exercise::Parse( async, system, path, items, meta );
    return true;
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
        const std::string name = Get< std::string >( tree, "content.name" );
        const std::string description = Get< std::string >( tree, "content.description" );
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
    {
        FileSystem_ABC::T_Predicate predicate = boost::bind( &FillItems, boost::ref( async ), boost::cref( system_ ), _1, boost::ref( items_ ), meta.get() );
        system_.Walk( path_, false, predicate );
    }
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
Package_ABC::T_Item Package::Find( size_t id, bool installed ) const
{
    T_Items::const_iterator it = FindItem( items_, id, installed );
    return it == items_.end() ? T_Item() : *it;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
Package_ABC::T_Item Package::Find( const Item_ABC& item, bool installed ) const
{
    T_Items::const_iterator it = FindItem( items_, item, installed );
    return it == items_.end() ? T_Item() : *it;
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Package_ABC::T_Item Package::Find( const Path& root, const std::string& checksum, bool installed ) const
{
    T_Items::const_iterator it = FindItem( items_, std::make_pair( root, checksum ), installed );
    return it == items_.end() ? T_Item() : *it;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: Compare
// Created: BAX 2012-09-10
// -----------------------------------------------------------------------------
bool Compare( const Package_ABC::T_Item& item, const std::string& type, const std::string& name, const std::string& checksum )
{
    return item->GetType()     == type
        && item->GetName()     == name
        && item->GetChecksum() == checksum;
}
}

// -----------------------------------------------------------------------------
// Name: Package::Find
// Created: BAX 2012-09-18
// -----------------------------------------------------------------------------
Package_ABC::T_Item Package::Find( const std::string& type, const std::string& name, const std::string& checksum ) const
{
    T_Items::const_iterator it = std::find_if( items_.begin(), items_.end(), boost::bind( &Compare, _1, type, name, checksum ) );
    return it == items_.end() ? T_Item() : *it;
}

namespace
{
void AddItemPath( boost::mutex& access, std::vector< Path >& list, const Path& path )
{
    boost::lock_guard< boost::mutex > lock( access );
    list.push_back( path );
}
}

// -----------------------------------------------------------------------------
// Name: Package::Install
// Created: BAX 2012-09-24
// -----------------------------------------------------------------------------
void Package::Install( Async& io, const Path& tomb, const T_Items& install )
{
    if( install.empty() )
        return;

    system_.MakePaths( path_ );
    Async async( pool_ );
    boost::mutex access;
    std::vector< Path > paths;
    PathOperand op = boost::bind( &AddItemPath, boost::ref( access ), boost::ref( paths ), _1 );
    BOOST_FOREACH( const T_Items::value_type& item, install )
        async.Go( boost::bind( &Item_ABC::Install, item, boost::ref( io ), boost::cref( system_ ),
                  tomb, path_, boost::cref( *this ), boost::cref( install ), op ) );
    async.Join();

    BOOST_FOREACH( const Path& path, paths )
        FillItems( async, system_, path, items_, 0 );
    async.Join();

    std::sort( items_.begin(), items_.end(), &ItemOrder );
    Identify( *this );
}

// -----------------------------------------------------------------------------
// Name: Package::Install
// Created: BAX 2012-05-24
// -----------------------------------------------------------------------------
void Package::Install( Async& io, const Path& tomb, const Package_ABC& src, const std::vector< size_t >& ids )
{
    T_Items install;
    BOOST_FOREACH( size_t id, ids )
        for( T_Item item = src.Find( id, true ); item; item.reset() )
            install.push_back( item );
    Install( io, tomb, install );
}

namespace
{
bool IsItemIn( const std::vector< size_t >& list, const Package_ABC::T_Item& item, bool unlinked )
{
    BOOST_FOREACH( const size_t& id, list )
        if( ( !unlinked || !item->IsLinked() ) && item->Compare( id ) )
            return true;
    return false;
}
template< typename T, typename U >
void RemoveItems( T& list, U predicate )
{
    list.erase( std::remove_if( list.begin(), list.end(), predicate ), list.end() );
}
}

// -----------------------------------------------------------------------------
// Name: Package::Uninstall
// Created: BAX 2012-05-25
// -----------------------------------------------------------------------------
void Package::Uninstall( Async& io, const Path& tomb, const std::vector< size_t >& ids )
{
    Async async( pool_ );
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        if( IsItemIn( ids, item, false ) )
            async.Go( boost::bind( &Item_ABC::Uninstall, item, boost::ref( io ), boost::cref( system_ ), tomb ) );
    async.Join();
    RemoveItems( items_, boost::bind( &IsItemIn, boost::cref( ids ), _1, true ) );
    Identify( *this );
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
        if( !item->IsInstalled() )
            continue;
        if( !item->IsValid() )
            continue;
        if( offset-- > 0 )
            continue;
        reply.push_back( item->GetName() );
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
        reply += item->IsExercise() && item->IsInstalled() && item->IsValid();
    return reply;
}

namespace
{
Tree Link( const Package_ABC::T_Item& item, const Package_ABC& pkg, bool recurse )
{
    Tree dst;
    if( item )
        item->Link( dst, pkg, recurse );
    return dst;
}
}

// -----------------------------------------------------------------------------
// Name: Package::LinkExercise
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Package::LinkExercise( const std::string& name )
{
    return Link( Find( Dependency( "exercise", name ), true ), *this, true );
}

namespace
{
void Link( Tree& dst, const Tree& src, const std::string& key, const Package& pkg )
{
    const Path root = Utf8( Get< std::string >( src, key + ".root"  ) );
    const std::string checksum = Get< std::string >( src, key + ".checksum" );
    Package_ABC::T_Item item = pkg.Find( root, checksum, false );
    if( item )
        item->Link( dst, pkg, false );
}
}

// -----------------------------------------------------------------------------
// Name: Package::LinkItem
// Created: BAX 2012-09-19
// -----------------------------------------------------------------------------
Tree Package::LinkItem( Item_ABC& item )
{
    Tree dst;
    item.Link( dst, *this, false );
    return dst;
}

// -----------------------------------------------------------------------------
// Name: Package::LinkItem
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
Tree Package::LinkItem( const Tree& tree )
{
    Tree dst;
    Link( dst, tree, "exercise", *this );
    Link( dst, tree, "terrain", *this );
    Link( dst, tree, "model", *this );
    return dst;
}

namespace
{
bool Unlink( Async& async, const FileSystem_ABC& system, const Tree& src, const std::string& key, const Package_ABC& pkg )
{
    const boost::optional< std::string > name = src.get_optional< std::string >( key + ".name" );
    if( name == boost::none )
        return false;
    Package_ABC::T_Item item = pkg.Find( Dependency( key, *name ), false );
    if( !item )
        return false;
    return item->Unlink( async, system );
}

bool IsOrphaned( const Package_ABC::T_Item& item )
{
    return !item->IsInstalled() && !item->IsLinked();
}
}

// -----------------------------------------------------------------------------
// Name: Package::UnlinkItem
// Created: BAX 2012-06-06
// -----------------------------------------------------------------------------
bool Package::UnlinkItem( Async& async, const Tree& src )
{
    bool modified = false;
    modified |= Unlink( async, system_, src, "exercise", *this );
    modified |= Unlink( async, system_, src, "terrain", *this );
    modified |= Unlink( async, system_, src, "model", *this );
    if( modified )
        RemoveItems( items_, &IsOrphaned );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: Package::GetSize
// Created: BAX 2012-07-19
// -----------------------------------------------------------------------------
size_t Package::GetSize() const
{
    size_t sum = 0;
    BOOST_FOREACH( const T_Items::value_type& item, items_ )
        sum += item->GetSize();
    return sum;
}

// -----------------------------------------------------------------------------
// Name: Package::Download
// Created: BAX 2012-09-11
// -----------------------------------------------------------------------------
void Package::Download( web::Chunker_ABC& dst, const Item_ABC& item )
{
    item.Download( system_, dst) ;
}