// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Package.h"

#include "runtime/win32/Api_ABC.h"
#include "runtime/Async.h"
#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"
#include "web/Configs.h"
#include "web/Chunker_ABC.h"
#include "web/HttpException.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/lock_guard.hpp>

#include <tools/Version.h>

using namespace host;
using namespace property_tree;
using runtime::Async;
using runtime::FileSystem_ABC;
using runtime::Pool_ABC;
using runtime::Utf8;

namespace
{
    typedef std::function< void( const Path& ) > PathOperand;
}

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
    virtual void        CheckDependencies( const Package_ABC& dst, const Package::T_Items& targets ) const = 0;
    virtual void        Install( Async& async, const FileSystem_ABC& fs, const Path& tomb,
                                 const Path& output, const Package_ABC& dst,
                                 const PathOperand& operand, bool move, bool replace ) const = 0;
    virtual void        Uninstall( Async& async, const FileSystem_ABC& fs, const Path& dst ) = 0;
    virtual void        Reinstall( const FileSystem_ABC& fs ) = 0;
    virtual void        Link( Tree& tree, const Package_ABC& ref, bool recurse ) = 0;
    virtual bool        Unlink( Async& async, const FileSystem_ABC& fs ) = 0;
    virtual void        Download( const FileSystem_ABC& fs, web::Chunker_ABC& dst ) const = 0;
};

namespace
{
    struct Metadata
    {
        Metadata( bool discard, const std::string& package, const std::string& version )
            : discard_ ( discard )
            , package_ ( package )
            , version_ ( version )
            , checksum_()
            , links_   ( 0 )
            , size_    ( 0 )
        {
            // NOTHING
        }

        Metadata()
            : discard_( false )
            , links_  ( 0 )
            , size_   ( 0 )
        {
            // NOTHING
        }

        std::string GetVersion() const
        {
            return version_;
        }

        std::string GetChecksum() const
        {
           return checksum_;
        }

        size_t GetSize() const
        {
           return size_;
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
            tree.put( "checksum", checksum_ );
            tree.put( "size", size_ );
        }

        void Finalize( const std::string& checksum, size_t size )
        {
           checksum_ = checksum;
           size_ = size;
        }

        static std::string GetFilename()
        {
            return "metadata.tag";
        }

        static std::string GetVersion( const Tree& tree, const std::string& key )
        {
            return Get< std::string >( tree, key, "Unversioned" );
        }

        void SetVersion( const std::string& version )
        {
            version_ = version;
        }

        static Metadata Reload( const FileSystem_ABC& fs, const Path& root )
        {
            return Metadata( FromJson( fs.ReadFile( root / GetFilename() ) ) );
        }

        void Save( const FileSystem_ABC& fs, const Path& root, bool force ) const
        {
            if( force || !discard_ )
                fs.WriteFile( root / GetFilename(), ToJson( GetProperties() ) );
        }

        void TryUninstall( Async& async, const FileSystem_ABC& fs, const Path& root, const Path& tomb )
        {
            tomb_ = tomb;
            if( IsLinked() )
                Save( fs, root, false );
            TryKill( async, fs, root );
        }

        void Reinstall( const FileSystem_ABC& fs, const Path& root )
        {
            tomb_.clear();
            Save( fs, root, false );
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

        bool Unlink( Async& async, const FileSystem_ABC& fs, const Path& root )
        {
            links_--;
            return TryKill( async, fs, root );
        }

    private:
        Metadata( const Tree& tree )
            : discard_ ( false )
            , package_ ( Get< std::string >( tree, "package" ) )
            , version_ ( GetVersion( tree, "version" ) )
            , checksum_( Get< std::string >( tree, "checksum" ) )
            , tomb_    ( Utf8( Get< std::string >( tree, "tomb" ) ) )
            , links_   ( 0 )
            , size_    ( Get< size_t >( tree, "size", 0 ) )
        {
            // NOTHING
        }

        Metadata& operator=( const Metadata& );

        bool TryKill( Async& async, const FileSystem_ABC& fs, const Path& root )
        {
            if( IsLinked() || IsInstalled() )
                return false;
            const Path next = fs.MakeAnyPath( tomb_ );
            fs.Rename( root, next / "_" );
            Post( async, boost::bind( &FileSystem_ABC::Remove, &fs, next ) );
            return true;
        }

        const bool discard_;
        const std::string package_;
        std::string version_;
        std::string checksum_;
        Path tomb_;
        size_t links_;
        size_t size_;
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
        return FindItem( list, item, false ) != list.end();
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

    bool EndWith( Path suffix, Path path )
    {
        for( ; !suffix.empty(); suffix.remove_filename(), path.remove_filename() )
            if( path.empty() )
                return false;
            else if( suffix.filename() != path.filename() )
                return false;
        return true;
    }

    struct Item : Package_ABC::Item_ABC
    {
        Item( const FileSystem_ABC& fs, const Path& root, size_t id, const std::string& name, const std::string& date, const Metadata* meta )
            : root_ ( root )
            , id_   ( id )
            , name_ ( name )
            , date_ ( date )
            , meta_ ( meta ? *meta : Metadata::Reload( fs, root ) )
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
            tree.put( "id", id_ );
            tree.put( "type", GetType() );
            tree.put( "name", Utf8( name_ ) );
            tree.put( "date", date_ );
            meta_.SaveTo( tree );
            if( !action_.empty() )
                tree.put( "action", action_ );
            if( !error_.empty() )
                tree.put( "error", error_ );
            return tree;
        }

        virtual size_t GetSize() const
        {
            return meta_.GetSize();
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
            return meta_.GetChecksum();
        }

        virtual void SetVersion( const std::string& version )
        {
            meta_.SetVersion( version );
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
            return pair.first == root_ && pair.second == GetChecksum();
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
            else if( GetChecksum() != next->GetChecksum() )
                action_ = "update";
        }

        virtual FileSystem_ABC::T_Predicate IsItemFile( const Path& /*root*/ ) const
        {
            const auto meta = Metadata::GetFilename();
            return [=]( const Path& path )
            {
                return !EndWith( meta, path );
            };
        }

        void MakeChecksum( const FileSystem_ABC& fs )
        {
            if( !GetChecksum().empty() )
               return;
            size_t read;
            const Path root = root_ / GetSuffix();
            const std::string checksum = fs.Checksum( root, IsItemFile( root ), read );
            meta_.Finalize( checksum, read );
            meta_.Save( fs, root_, false );
        }

        static bool Copy( const FileSystem_ABC& fs, const Path& path, const Path& dst )
        {
            return fs.Rename( path, dst / Path( path ).filename() );
        }

        void CheckDependencies( const Package_ABC& dst, const Package::T_Items& targets ) const
        {
            BOOST_FOREACH( const T_Dependencies::value_type& dep, GetDependencies() )
                if( !dst.Find( *dep, true ) && !HasItem( targets, *dep ) )
                    throw web::HttpException( web::BAD_REQUEST, "Missing dependency "
                        + Utf8( dep->GetName() ) + " on package " + Utf8( GetName() ) );
        }

        void Install( Async& async, const FileSystem_ABC& fs, const Path& tomb, const Path& root, const Package_ABC& dst,
                      const PathOperand& operand, bool move, bool replace ) const
        {
            Package_ABC::T_Item old = dst.Find( *this, false );
            const std::string checksum = GetChecksum();
            if( old && old->GetChecksum() == checksum )
                return old->Reinstall( fs );

            const Path output = fs.MakeAnyPath( root );
            if( move )
            {
                fs.MakePaths( output );
                fs.Walk( root_, false, boost::bind( &Item::Copy, boost::cref( fs ), _1, output ) );
            }
            else
            {
                const Path sub =  output / GetSuffix();
                fs.MakePaths( sub );
                fs.CopyDirectory( root_ / GetSuffix(), sub );
            }
            meta_.Save( fs, output, true );
            if( old && replace )
                old->Uninstall( async, fs, tomb );
            operand( output );
        }

        void Uninstall( Async& async, const FileSystem_ABC& fs, const Path& dst )
        {
            meta_.TryUninstall( async, fs, root_, dst );
        }

        void Reinstall( const FileSystem_ABC& fs )
        {
            meta_.Reinstall( fs, root_ );
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
            tree.put( prefix + ".checksum", GetChecksum() );
            if( recurse )
                BOOST_FOREACH( const T_Dependencies::value_type& it, GetDependencies() )
                {
                    Package_ABC::T_Item next = ref.Find( *it, true );
                    if( next )
                        next->Link( tree, ref, false );
                }
            meta_.Link();
        }

        bool Unlink( Async& async, const FileSystem_ABC& fs )
        {
            return meta_.Unlink( async, fs, root_ );
        }

        void Download( const FileSystem_ABC& fs, web::Chunker_ABC& dst ) const
        {
            dst.SetName( Utf8( name_ ) + ".tar.gz" );
            dst.SetHeader( "Content-Type", "application/x-tar" );
            dst.SetHeader( "Original-Content-Length", boost::lexical_cast< std::string >( GetSize() ) );
            io::Writer_ABC& io = dst.OpenWriter();
            FileSystem_ABC::T_Packer packer = fs.Pack( io, runtime::ARCHIVE_FMT_TAR_GZ );
            packer->Pack( root_, IsItemFile( GetSuffix() ) );
            packer->PackEntry( "signature", 0, 0 );
        }

    protected:
        const Path root_;
        const size_t id_;
        const Path name_;
        const std::string date_;
        Metadata meta_;
        std::string action_;
        std::string error_;
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
    void AttachItem( Async& async, const FileSystem_ABC& fs, Package::T_Items& items, const T& item )
    {
        items.push_back( item );
        async.Post( boost::bind( &Item::MakeChecksum, item, boost::cref( fs ) ) );
    }

    template< typename T >
    void AttachSimple( Async& async, const Path& path, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta )
    {
        AttachItem( async, fs, items, boost::make_shared< T >( fs, root, path, items.size(), meta ) );
    }

    template< typename T >
    bool AttachExtended( Async& async, const Path& path, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta )
    {
        AttachItem( async, fs, items, boost::make_shared< T >( fs, root, path, items.size(), meta, FromXml( fs.ReadFile( path ) ) ) );
        return true;
    }

    struct Model : public Item
    {
        Model( const FileSystem_ABC& fs, const Path& root, const Path& file, size_t id, const Metadata* meta )
            : Item( fs, root, id, Utf8( Path( file ).remove_filename().remove_filename().filename() ), Format( fs.GetLastWrite( file ) ), meta )
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

        static bool Operand( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta,
                             const Path& path )
        {
            const Path target = path / "decisional" / "decisional.xml";
            if( fs.IsFile( target ) )
                AttachSimple< Model >( async, target, fs, root, items, meta );
            return true;
        }

        static void Parse( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta )
        {
            FileSystem_ABC::T_Predicate op = boost::bind( &Model::Operand,
                boost::ref( async ), boost::cref( fs ), boost::cref( root ), boost::ref( items ), meta, _1 );
            fs.Walk( root / "data" / "models", false, op );
        }
    };

    struct Terrain : public Item
    {
        Terrain( const FileSystem_ABC& fs, const Path& root, const Path& file, size_t id, const Metadata* meta )
            : Item( fs, root, id, GetFilename( file, "terrains" ), Format( fs.GetLastWrite( file ) ), meta )
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

        static bool Operand( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta,
                             const Path& path )
        {
            std::wstring filename = path.filename().wstring();
            boost::to_lower( filename );
            if( filename == L"terrain.xml" )
                AttachSimple< Terrain >( async, path, fs, root, items, meta );
            return true;
        }

        static void Parse( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta )
        {
            FileSystem_ABC::T_Predicate op = boost::bind( &Terrain::Operand,
                boost::ref( async ), boost::cref( fs ), boost::cref( root ), boost::ref( items ), meta, _1 );
            fs.Walk( root / "data" / "terrains", true, op );
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

    typedef std::map< std::string, std::string > T_Sides;

    T_Sides GetSides( const FileSystem_ABC& fs, const Path& path )
    {
        if( !fs.Exists( path ) )
            return T_Sides();
        const Tree src = FromXml( fs.ReadFile( path ) );
        const auto opt = src.get_child_optional( "orbat.parties" );
        if( !opt )
            return T_Sides();
        T_Sides sides;
        BOOST_FOREACH( const auto& side, *opt )
            if( side.first == "party" )
                sides.insert( std::make_pair( Get< std::string >( side.second, "<xmlattr>.id" ), Get< std::string >( side.second, "<xmlattr>.name" ) ) );
        return sides;
    }

    Tree ConvertSides( const T_Sides& sides )
    {
        Tree tree;
        for( auto it = sides.begin(); it != sides.end(); ++it )
            tree.put( it->first, it->second );
        return tree;
    }

    typedef web::session::Config::T_Profiles T_Profiles;

    T_Profiles GetProfiles( const FileSystem_ABC& fs, const Path& path )
    {
        if( !fs.Exists( path ) )
            return T_Profiles();
        const Tree src = FromXml( fs.ReadFile( path ) );
        const auto opt = src.get_child_optional( "profiles" );
        if( !opt )
            return T_Profiles();
        T_Profiles profiles;
        BOOST_FOREACH( const auto& it, *opt )
            if( it.first == "profile" )
                profiles.insert( web::session::Profile(
                    Get< std::string >( it.second, "<xmlattr>.name" ),
                    Get< std::string >( it.second, "<xmlattr>.password" ) ) );
        return profiles;
    }

    struct Exercise : public Item
    {
        Exercise( const FileSystem_ABC& fs, const Path& root, const Path& file, size_t id, const Metadata* meta, const Tree& more )
            : Item     ( fs, root, id, GetFilename( file, "exercises" ), Format( fs.GetLastWrite( file ) ), meta )
            , briefing_( Get< std::string >( more, "exercise.meta.briefing.text" ) )
            , model_   ( Get< std::string >( more, "exercise.model.<xmlattr>.dataset" ) )
            , terrain_ ( Get< std::string >( more, "exercise.terrain.<xmlattr>.name" ) )
            , sides_   ( GetSides( fs, file.parent_path() / "orbat.xml" ) )
            , profiles_( GetProfiles( fs, file.parent_path() / "profiles.xml" ) )
        {
            // NOTHING
        }

        virtual Tree GetProperties() const
        {
            Tree tree = Item::GetProperties();
            tree.put( "briefing", briefing_ );
            tree.put( "model", model_ );
            tree.put( "terrain", terrain_ );
            tree.put_child( "sides", ConvertSides( sides_ ) );
            web::session::WriteProfileConfig( tree, "profiles.", profiles_ );
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

        FileSystem_ABC::T_Predicate IsItemFile( const Path& root ) const
        {
            const auto predicate = Item::IsItemFile( root );
            return [=]( const Path& path )
            {
                return predicate( path ) && !BeginWith( root / "sessions", path );
            };
        }

        static bool Operand( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta,
                             const Path& path )
        {
            if( path.filename() == "exercise.xml" )
                AttachExtended< Exercise >( async, path, fs, root, items, meta );
            return true;
        }

        static void Parse( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items, const Metadata* meta )
        {
            FileSystem_ABC::T_Predicate op = boost::bind( &Exercise::Operand,
                boost::ref( async ), boost::cref( fs ), boost::cref( root ), boost::ref( items ), meta, _1 );
            fs.Walk( root / "exercises", true, op );
        }

        const std::string briefing_;
        const std::string model_;
        const std::string terrain_;
        const T_Sides     sides_;
        const T_Profiles  profiles_;
    };

    typedef std::vector< std::string > T_Tokens;

    T_Tokens Split( const std::string& value )
    {
        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, value, boost::is_any_of( "." ) );
        return tokens;
    }

    std::string Splice( const T_Tokens& src, size_t end )
    {
        const T_Tokens slice( src.begin(), src.begin() + std::min( end, src.size() ) );
        return boost::algorithm::join( slice, "." );
    }

    std::string GetVersion( const std::string& version )
    {
        return version.empty() ? "Unversioned" : version;
    }

    std::string GetClient( const Path& root )
    {
        const auto app = root / "gaming_app.exe";
        const bool x64 = runtime::Api_ABC::Is64BitBinary( app.wstring().data() );
        return std::string( "client" ) + ( x64 ? "64" : "32" );
    };

    struct Client : public Item
    {
        Client( const FileSystem_ABC& fs, const Path& root, const Path& file, size_t id )
            : Item( fs, root, id, "gaming", Format( fs.GetLastWrite( file ) ), nullptr )
            , type_( GetClient( root ) )
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

        static void Parse( Async& async, const FileSystem_ABC& fs, const Path& root, Package::T_Items& items )
        {
            const Path file = root / "gaming_app.exe";
            if( fs.IsFile( file ) )
            {
                const auto item = boost::make_shared< Client >( fs, root, file, items.size() );
                item->SetVersion( GetVersion( Splice( Split( tools::GetAppVersion( file.wstring(), "Unversioned" ) ), 3 ) ) );
                AttachItem( async, fs, items, item );
            }
        }

        const std::string type_;
    };
}

// -----------------------------------------------------------------------------
// Name: Package::Package
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
Package::Package( Pool_ABC& pool, const FileSystem_ABC& fs, const Path& path,
                  bool reference, bool replace )
    : pool_     ( pool )
    , fs_       ( fs )
    , path_     ( path )
    , reference_( reference )
    , replace_  ( replace )
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
// Name: Package::GetProperties
// Created: BAX 2012-09-25
// -----------------------------------------------------------------------------
Tree Package::GetPropertiesFrom( const Item_ABC& item ) const
{
    return item.GetProperties();
}

// -----------------------------------------------------------------------------
// Name: Package::GetPath
// Created: BAX 2012-05-31
// -----------------------------------------------------------------------------
Path Package::GetPath() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: Package::GetRoot
// Created: BAX 2012-10-02
// -----------------------------------------------------------------------------
Path Package::GetRoot( const Item_ABC& item ) const
{
    return item.GetRoot();
}

namespace
{
bool FillItems( Async& async, const FileSystem_ABC& fs, const Path& path, Package::T_Items& items, Metadata* meta )
{
    Model::Parse   ( async, fs, path, items, meta );
    Terrain::Parse ( async, fs, path, items, meta );
    Exercise::Parse( async, fs, path, items, meta );
    Client::Parse  ( async, fs, path, items );
    return true;
}
}

// -----------------------------------------------------------------------------
// Name: Package::Parse
// Created: BAX 2012-05-14
// -----------------------------------------------------------------------------
bool Package::Parse()
{
    std::unique_ptr< Metadata > meta;

    if( !reference_ )
    {
        const Path index = path_ / "content.xml";
        if( !fs_.IsFile( index ) )
            return false;

        const Tree tree = FromXml( fs_.ReadFile( index ) );
        const std::string name = Get< std::string >( tree, "content.name" );
        const std::string description = Get< std::string >( tree, "content.description" );
        if( name.empty() || description.empty() )
            return false;

        name_ = name;
        description_ = description;
        version_ = Metadata::GetVersion( tree, "content.version" );
        meta.reset( new Metadata( true, name_, version_ ) );
    }

    items_.clear();
    Async async( pool_ );
    if( reference_ )
        fs_.Walk( path_, false, boost::bind( &FillItems, boost::ref( async ),
                  boost::cref( fs_ ), _1, boost::ref( items_ ), meta.get() ) );
    else
        FillItems( async, fs_, path_, items_, meta.get() );
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
// Name: CompareChecksum
// Created: BAX 2012-09-10
// -----------------------------------------------------------------------------
bool CompareChecksum( const Package_ABC::T_Item& item, const std::string& type, const std::string& name, const std::string& checksum )
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
    T_Items::const_iterator it = std::find_if( items_.begin(), items_.end(), boost::bind( &CompareChecksum, _1, type, name, checksum ) );
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
void Package::InstallWith( Async& io, const Path& tomb, const T_Items& install, bool move )
{
    if( install.empty() )
        return;

    // before installing anything, check all packages are installable
    for( auto it = install.begin(); it != install.end(); ++it )
        (*it)->CheckDependencies( *this, install );

    fs_.MakePaths( path_ );
    Async async( pool_ );
    boost::mutex access;
    std::vector< Path > paths;
    PathOperand op = boost::bind( &AddItemPath, boost::ref( access ), boost::ref( paths ), _1 );
    BOOST_FOREACH( const T_Items::value_type& item, install )
        async.Post( boost::bind( &Item_ABC::Install, item, boost::ref( io ), boost::cref( fs_ ),
                  tomb, path_, boost::cref( *this ), op, move, replace_ ) );
    async.Join();

    BOOST_FOREACH( const Path& path, paths )
        FillItems( async, fs_, path, items_, 0 );
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
    {
        T_Item item = src.Find( id, true );
        if( !item )
            throw web::HttpException( web::BAD_REQUEST, "Missing package "
                + boost::lexical_cast< std::string >( id ) );
        install.push_back( item );
    }
    InstallWith( io, tomb, install, false );
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
            async.Post( boost::bind( &Item_ABC::Uninstall, item, boost::ref( io ), boost::cref( fs_ ), tomb ) );
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

Tree Package::GetExerciseProperties( const std::string& name )
{
    auto item = Find( Dependency( "exercise", name ), true );
    if( !item )
        return Tree();
    return GetPropertiesFrom( *item );
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
bool Unlink( Async& async, const FileSystem_ABC& fs, const Tree& src, const std::string& key, const Package_ABC& pkg )
{
    const auto name = src.get_optional< std::string >( key + ".name" );
    const auto checksum = src.get_optional< std::string >( key + ".checksum" );
    if( !name || !checksum )
        return false;
    auto item = pkg.Find( key, *name, *checksum );
    if( !item )
        return false;
    return item->Unlink( async, fs );
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
    modified |= Unlink( async, fs_, src, "exercise", *this );
    modified |= Unlink( async, fs_, src, "terrain", *this );
    modified |= Unlink( async, fs_, src, "model", *this );
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
    item.Download( fs_, dst) ;
}

Package::T_Item Package::MakeDependency( const std::string& type, const std::string& name )
{
    return boost::make_shared< Dependency >( type, name );
}
