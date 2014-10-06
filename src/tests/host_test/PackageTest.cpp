// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test_pch.h"

#include "host/Package.h"
#include "runtime/Async.h"
#include "runtime/FileSystem.h"
#include "runtime/PropertyTree.h"
#include "runtime/Scoper.h"
#include "web/Chunker_ABC.h"
#include "web/HttpException.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <set>

#include "MockFileSystem.h"
#include "MockLog.h"
#include "MockPool.h"

using namespace host;
using namespace mocks;
using namespace property_tree;
using runtime::Async;

namespace
{
bool BeginWith( const Path& prefix, const Path& path )
{
    Path::const_iterator a = prefix.begin();
    Path::const_iterator b = path.begin();
    for( ; a != prefix.end(); ++a, ++b )
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

typedef std::vector< Path > T_Paths;

std::string MakeExerciseData( const std::string& model, const std::string& terrain )
{
    return "<exercise><model dataset=\"" + model + "\"/><terrain name=\"" + terrain + "\"/></exercise>";
}

std::string MakeMetadata( const std::string& checksum )
{
    return
"{"
"    \"package\": \"some_package\","
"    \"version\": \"trunk\","
"    \"checksum\": \"" + checksum + "\""
"}";
}

size_t CountItemType( const Tree& src, const std::string& type )
{
    size_t count = 0;
    const auto& sub = src.find( "items" )->second;
    for( auto it = sub.begin(); it != sub.end(); ++it )
        count += it->second.get< std::string >( "type" ) == type;
    return count;
}

void CheckCounts( const Tree& data, size_t models, size_t terrains, size_t exercises )
{
    BOOST_CHECK_EQUAL( CountItemType( data, "model" ), models );
    BOOST_CHECK_EQUAL( CountItemType( data, "terrain" ), terrains );
    BOOST_CHECK_EQUAL( CountItemType( data, "exercise" ), exercises );
}

template< bool replace = true, bool gaming = false >
struct Fixture
{
    Fixture()
        : idx    ( 0 )
        , install( pool, fs, GetFileIndex(), true,  replace )
        , cache  ( pool, fs, GetFileIndex(), false, replace )
    {
        if( !gaming )
            MOCK_EXPECT( fs.IsFile ).with( boost::bind( &EndWith, "gaming_app.exe", _1 ) ).returns( false );
        MOCK_EXPECT( fs.Exists ).with( boost::bind( &EndWith, "orbat.xml", _1 ) ).returns( false );
        MOCK_EXPECT( fs.Exists ).with( boost::bind( &EndWith, "profiles.xml", _1 ) ).returns( false );
    }

    size_t idx;
    MockPool pool;
    MockFileSystem fs;
    Package install;
    Package cache;

    std::string GetFileIndex()
    {
        return boost::lexical_cast< std::string >( idx++ );
    }


    void AddItem( T_Paths& dst, bool ref, const Path& root, const Path& prefix, const Path& name, const Path& suffix,
                  const std::string& checksum, const std::string& contents, bool glob )
    {
        const Path data = prefix.empty() ? root : root / prefix;
        const Path file = name.empty() ? data / suffix : data / name / suffix;
        if( glob )
        {
            MOCK_EXPECT( fs.Walk ).once().with( data, mock::any, boost::bind( &MockFileSystem::Apply, &fs, _1, boost::assign::list_of( file ) ) );
            if( ref )
                MOCK_EXPECT( fs.Walk ).exactly( 2 ).with( boost::bind( &BeginWith, root, _1 ), mock::any, mock::any );
        }
        else
        {
            if( !name.empty() )
                MOCK_EXPECT( fs.Walk ).once().with( data, false, boost::bind( &MockFileSystem::Apply, &fs, _1, boost::assign::list_of( data / name ) ) );
            MOCK_EXPECT( fs.IsFile ).once().with( file ).returns( true );
            if( ref )
                MOCK_EXPECT( fs.Walk ).exactly( 2 ).with( boost::bind( &BeginWith, root, _1 ), mock::any, mock::any );
        }
        MOCK_EXPECT( fs.GetLastWrite ).with( file ).returns( std::time_t() );
        MOCK_EXPECT( fs.ReadFile ).with( file ).returns( contents );
        if( !checksum.empty() )
        {
            MOCK_EXPECT( fs.ReadFile ).with( root / "metadata.tag" ).returns( "" );
            MOCK_EXPECT( fs.WriteFile ).with( root / "metadata.tag", mock::any ).returns( true );
            MOCK_EXPECT( fs.Checksum ).with( data / name, mock::any, mock::any ).returns( checksum );
        }
        dst.push_back( file );
    }

    void AddModel( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum )
    {
        AddItem( dst, ref, root, "data/models", name, "decisional/decisional.xml", checksum, "", false );
        if( gaming )
            MOCK_EXPECT( fs.IsFile ).with( root / "gaming_app.exe" ).returns( false );
    }

    void AddTerrain( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum )
    {
        AddItem( dst, ref, root, "data/terrains", name, "Terrain.xml", checksum, "", true );
        if( gaming )
            MOCK_EXPECT( fs.IsFile ).with( root / "gaming_app.exe" ).returns( false );
    }

    void AddExercise( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum, const std::string& model, const std::string& terrain )
    {
        AddItem( dst, ref, root, "exercises", name, "exercise.xml", checksum, MakeExerciseData( model, terrain ), true );
        if( gaming )
            MOCK_EXPECT( fs.IsFile ).with( root / "gaming_app.exe" ).returns( false );
    }

    void AddClient( T_Paths& dst, bool ref, const Path& root, const std::string& checksum )
    {
        AddItem( dst, ref, root, "", "", "gaming_app.exe", checksum, "", false );
        MOCK_EXPECT( fs.Walk ).once().with( root / "exercises", mock::any, mock::any );
    }

    Path Decompose( const Path& path, size_t offset )
    {
        Path rpy;
        for( Path::const_iterator it = path.begin(); it != path.end() && offset; ++it, --offset )
            rpy = rpy / *it;
        return rpy;
    }

    T_Paths GetItemRoots( const T_Paths& src )
    {
        T_Paths rpy;
        std::set< Path > roots;
        BOOST_FOREACH( const Path& it, src )
        {
            const Path sub = Decompose( it, 2 );
            if( roots.insert( sub ).second )
                rpy.push_back( sub );
        }
        return rpy;
    }

    void AddPackageDescriptor( const Path& root, const std::string& name, const std::string& description, const std::string& version )
    {
        const Path content = root / "content.xml";
        MOCK_EXPECT( fs.IsFile ).with( content ).returns( true );
        Tree next;
        next.put( "content.name", name );
        next.put( "content.description", description );
        next.put( "content.version", version );
        MOCK_EXPECT( fs.ReadFile ).with( content ).returns( ToXml( next ).c_str() );
    }

    Path GetItemRoot( const Path& root, bool ref )
    {
        return ref ? root / GetFileIndex() : root;
    }

    void AddSomePackages( const Package_ABC& pkg, bool ref )
    {
        T_Paths dst;
        const Path root = pkg.GetPath();
        if( !ref )
            AddPackageDescriptor( root, "some_name", "some_description", "some_version" );
        AddModel   ( dst, ref, GetItemRoot( root, ref ), "ada", "01234567" );
        AddTerrain ( dst, ref, GetItemRoot( root, ref ), "egypt", "12345678" );
        AddExercise( dst, ref, GetItemRoot( root, ref ), "shore", "23456789", "ada", "egypt" );

        if( !ref )
            return;

        AddExercise( dst, ref, GetItemRoot( root, ref ), "castle", "34567890", "ada", "avalon" );
        MOCK_EXPECT( fs.Walk ).once().with( root, false, boost::bind( &MockFileSystem::Apply, &fs, _1, GetItemRoots( dst ) ) );
    }

    const Tree& GetItem( const Tree& src, const std::string& type, const std::string& name )
    {
        const Tree& sub = src.find( "items" )->second;
        for( Tree::const_iterator it = sub.begin(); it != sub.end(); ++it )
            if( it->second.get< std::string >( "type" ) == type )
                if( it->second.get< std::string >( "name" ) == name )
                    return it->second;
        throw std::runtime_error( "unknown item" );
    }

    Tree CheckItem( const Tree& data, const std::string& type, const std::string& name, const std::string& checksum )
    {
        const Tree item = GetItem( data, type, name );
        BOOST_CHECK_EQUAL( item.get< std::string >( "checksum" ), checksum );
        return item;
    }

    Tree CheckExercise( const Tree& data, const std::string& name, const std::string& checksum, const std::string& model, const std::string& terrain )
    {
        const Tree item = CheckItem( data, "exercise", name, checksum );
        BOOST_CHECK_EQUAL( item.get< std::string >( "model" ), model );
        BOOST_CHECK_EQUAL( item.get< std::string >( "terrain" ), terrain );
        return item;
    }

    void CheckSomePackages( const Package_ABC& pkg, bool ref )
    {
        const size_t numExercises = 1 + !!ref;
        const Tree data = pkg.GetProperties();
        CheckCounts( data, 1, 1, numExercises );
        CheckItem( data, "model", "ada", "01234567" );
        CheckItem( data, "terrain", "egypt", "12345678" );
        CheckExercise( data, "shore", "23456789", "ada", "egypt" );

        BOOST_CHECK_EQUAL( pkg.CountExercises(), size_t( 1 ) );
        const Package_ABC::T_Exercises list = pkg.GetExercises( 0, INT_MAX );
        BOOST_CHECK_EQUAL( list.size(), size_t( 1 ) );
        BOOST_CHECK_EQUAL( list[0], "shore" );

        if( !ref )
            return;
        const Tree castle = CheckExercise( data, "castle", "34567890", "ada", "avalon" );
        BOOST_CHECK( !castle.get< std::string >( "error" ).empty() );
    }

    void AddCheckPackages( Package_ABC& pkg, bool reference )
    {
        AddSomePackages( pkg, reference );
        pkg.Parse();
        pkg.Identify( pkg );
        CheckSomePackages( pkg, reference );
    }

    Path InstallPackage( const Package_ABC& input,
                         const std::string& type,
                         const std::string& name,
                         const std::string& checksum,
                         const std::string& model = std::string(),
                         const std::string& terrain = std::string() )
    {
        const Path root = install.GetPath();
        const Path sub = root / GetFileIndex();
        MOCK_EXPECT( fs.MakePaths ).with( root );
        MOCK_EXPECT( fs.MakeAnyPath ).once().with( root ).returns( sub );
        MOCK_EXPECT( fs.MakePaths ).once().with( boost::bind( &BeginWith, sub, _1 ) );
        MOCK_EXPECT( fs.CopyDirectory ).once();
        T_Paths dst;
        size_t idx;
        if( type == "model" )
        {
            AddModel( dst, true, sub, name, checksum );
            idx = 0;
        }
        else if( type == "terrain" )
        {
            AddTerrain( dst, true, sub, name, checksum );
            idx = 1;
        }
        else if( type == "exercise" )
        {
            AddExercise( dst, true, sub, name, checksum, model, terrain );
            idx = 2;
        }
        Async async( pool );
        const auto tomb = root / GetFileIndex();
        install.Install( async, tomb, input, boost::assign::list_of( idx ) );
        return tomb;
    }

    void InstallSomePackage()
    {
        AddCheckPackages( cache, false );
        InstallPackage( cache, "model", "ada", "01234567" );
        InstallPackage( cache, "terrain", "egypt", "12345678" );
        InstallPackage( cache, "exercise", "shore", "23456789", "ada", "egypt" );

        const Tree data = install.GetProperties();
        CheckCounts( data, 1, 1, 1 );
        CheckItem( data, "model", "ada", "01234567" );
    }

    void ExpectItemRemoval( const Path& root )
    {
        const Path sub = root / GetFileIndex();
        MOCK_EXPECT( fs.MakeAnyPath ).once().with( root ).returns( sub );
        MOCK_EXPECT( fs.Rename ).once().with( boost::bind( &BeginWith, install.GetPath(), _1 ), boost::bind( &BeginWith, sub, _1 ) ).returns( true );
        MOCK_EXPECT( fs.Remove ).with( sub ).returns( true );
    }

    void InstallWith( const std::string& model,    const std::string& modelChecksum,
                      const std::string& terrain,  const std::string& terrainChecksum,
                      const std::string& exercise, const std::string& exerciseChecksum )
    {
        T_Paths dst;
        const auto root = install.GetPath();
        AddModel   ( dst, true, GetItemRoot( root, true ), model, modelChecksum );
        AddTerrain ( dst, true, GetItemRoot( root, true ), terrain, terrainChecksum );
        AddExercise( dst, true, GetItemRoot( root, true ), exercise, exerciseChecksum, model, terrain );
        MOCK_EXPECT( fs.Walk ).once().with( root, false, boost::bind( &MockFileSystem::Apply, &fs, _1, GetItemRoots( dst ) ) );
        install.Parse();
        mock::verify();
    }

    void Install( const std::string& exercise, const std::string& checksum )
    {
        InstallWith( "ada", "v1", "ter", "v1", exercise, checksum );
    }

    Path UploadAndInstall( const std::string& exercise, const std::string& checksum )
    {
        T_Paths dst;
        Package upload( pool, fs, GetFileIndex(), false, true );
        const auto root = upload.GetPath();
        AddPackageDescriptor( root, "upload name", "upload description", "upload version" );
        AddModel   ( dst, false, GetItemRoot( root, false ), "ada", "v1" );
        AddTerrain ( dst, false, GetItemRoot( root, false ), "ter", "v1" );
        AddExercise( dst, false, GetItemRoot( root, false ), exercise, checksum, "ada", "ter" );
        upload.Parse();
        const auto tomb = InstallPackage( upload, "exercise", exercise, checksum, "ada", "ter" );
        mock::verify();
        return tomb;
    }
};
}

BOOST_FIXTURE_TEST_CASE( install_parses, Fixture<> )
{
    AddCheckPackages( install, true );
}

BOOST_FIXTURE_TEST_CASE( cache_parses, Fixture<> )
{
    AddCheckPackages( cache, false );
}

BOOST_FIXTURE_TEST_CASE( package_installs, Fixture<> )
{
    InstallSomePackage();
}

BOOST_FIXTURE_TEST_CASE( package_uninstalls, Fixture<> )
{
    InstallSomePackage();

    const Path dst = GetFileIndex();
    Async async( pool );
    install.Uninstall( async, dst, boost::assign::list_of( 42 ) );

    Tree data;
    data = install.GetProperties();
    CheckCounts( data, 1, 1, 1 );

    ExpectItemRemoval( dst );
    install.Uninstall( async, dst, boost::assign::list_of( 0 ) );

    data = install.GetProperties();
    CheckCounts( data, 0, 1, 1 );

    const Tree shore = CheckExercise( data, "shore", "23456789", "ada", "egypt" );
    BOOST_CHECK( !shore.get< std::string >( "error" ).empty() );
}

BOOST_FIXTURE_TEST_CASE( package_links, Fixture<> )
{
    InstallSomePackage();

    const Tree link = install.LinkExercise( "shore" );
    const Tree other = install.LinkItem( link );

    const Path dst = GetFileIndex();
    Async async( pool );
    install.Uninstall( async, dst, boost::assign::list_of( 0 ) );
    const Tree data = install.GetProperties();
    CheckCounts( data, 0, 1, 1 );
    mock::verify();

    install.UnlinkItem( async, other );
    mock::verify();

    ExpectItemRemoval( dst );
    install.UnlinkItem( async, link );
}

BOOST_FIXTURE_TEST_CASE( package_reinstall_silently, Fixture<> )
{
    InstallSomePackage();
    install.LinkExercise( "shore" );

    Async async( pool );
    install.Uninstall( async, GetFileIndex(), boost::assign::list_of( 2 ) );
    mock::verify();

    install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 2 ) );
}

BOOST_FIXTURE_TEST_CASE( installing_an_unknown_package_throws, Fixture<> )
{
    InstallSomePackage();

    Async async( pool );
    install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 0 ) );
    BOOST_CHECK_THROW( install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 51 ) ), web::HttpException );
    BOOST_CHECK_THROW( install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 1 )( 51 ) ), web::HttpException );
}

BOOST_FIXTURE_TEST_CASE( installing_a_package_missing_a_dependency_throws, Fixture<> )
{
    AddCheckPackages( cache, false );

    Async async( pool );
    BOOST_CHECK_THROW( install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 2 ) ), web::HttpException );
}

namespace
{
void CheckEgypt( bool& sentinel, const Tree& item, size_t size )
{
    BOOST_CHECK( !sentinel );
    sentinel = true;
    BOOST_CHECK_EQUAL( Get< std::string >( item, ".type" ), "exercise" );
    BOOST_CHECK_EQUAL( Get< Path >( item, ".name" ), Path( "worldwide/Egypt" ) );
    BOOST_CHECK_EQUAL( Get< std::string >( item, ".checksum" ), "1A741642" );
    BOOST_CHECK_EQUAL( Get< size_t >( item, ".size" ), size );
}
}

BOOST_AUTO_TEST_CASE( package_checksum_skip_unwanted_data )
{
    MockPool pool;
    MockLog log;
    runtime::FileSystem fs( log );
    const Path root = testOptions.GetDataPath( "packages" ).ToBoost();
    Package pkg( pool, fs, root, true, true );
    pkg.Parse();

    BOOST_CHECK_EQUAL( pkg.CountExercises(), size_t( 1 ) );
    const size_t size = 44040;
    BOOST_CHECK_EQUAL( pkg.GetSize(), size );
    const Tree tree = pkg.GetProperties();
    const std::vector< std::string > tokens = boost::assign::list_of( "items" );
    bool sentinel = false;
    Walk( tree, tokens.begin(), tokens.end(), boost::bind( &CheckEgypt, boost::ref( sentinel ), _1, size ) );
}

namespace
{
    struct VirtualDevice : public io::Writer_ABC
                         , public io::Reader_ABC
    {
        VirtualDevice()
        {
            // NOTHING
        }

        int Write( const void* data, size_t size )
        {
            const size_t previous = buffer_.size();
            buffer_.resize( previous + size, 0 );
            memcpy( &buffer_[previous], data, size );
            return static_cast< int >( size );
        }

        int Read( void* data, size_t size )
        {
            const size_t next = std::min( size, buffer_.size() );
            if( next )
                memcpy( data, &buffer_[0], next );
            buffer_.erase( buffer_.begin(), buffer_.begin() + next );
            return static_cast< int >( next );
        }

        std::vector< char > buffer_;
    };

    MOCK_BASE_CLASS( MockChunker, web::Chunker_ABC )
    {
        MOCK_METHOD( SetHeader, 2 );
        MOCK_METHOD( SetName, 1 );
        MOCK_METHOD( OpenWriter, 0 );
    };
}

BOOST_AUTO_TEST_CASE( package_pack_unpack )
{
    MockPool pool;
    MockLog log;
    runtime::FileSystem fs( log );
    const Path root = testOptions.GetDataPath( "packages" ).ToBoost();
    Package pkg( pool, fs, root, true, true );
    pkg.Parse();

    // compress package to virtual device
    auto item = pkg.FindId( 0 );
    BOOST_REQUIRE( item );
    VirtualDevice buffer;
    MockChunker chunker;
    MOCK_EXPECT( chunker.OpenWriter ).returns( boost::ref( buffer ) );
    MOCK_EXPECT( chunker.SetName );
    MOCK_EXPECT( chunker.SetHeader );
    pkg.Download( chunker, *item );

    // uncompress package to random dir, and check it against source
    const Path output = fs.MakeAnyPath( testOptions.GetDataPath( "." ).ToBoost() );
    const auto erase = runtime::Scoper( [&]{ fs.Remove( output ); } );
    const Path pkgdir = output / "0";
    auto unpacker = fs.Unpack( pkgdir, buffer, 0 );
    unpacker->Unpack();
    BOOST_CHECK( fs.IsFile( pkgdir / "signature" ) );
    BOOST_CHECK( !fs.IsFile( pkgdir / "metadata.tag" ) );
    Package dst( pool, fs, output, true, true );
    dst.Parse();
    BOOST_CHECK_EQUAL( dst.GetSize(), pkg.GetSize() );
    auto target = dst.FindId( 0 );
    BOOST_REQUIRE( target );
    BOOST_CHECK_EQUAL( ToJson( pkg.GetPropertiesFrom( *item ) ), ToJson( dst.GetPropertiesFrom( *target ) ) );
}

BOOST_FIXTURE_TEST_CASE( package_check_versioning_basics, Fixture<> )
{
    // setup & link v1 exercise
    Install( "exo", "v1" );
    CheckCounts( install.GetProperties(), 1, 1, 1 );
    const auto v1Link = install.LinkExercise( "exo" );
    BOOST_REQUIRE_EQUAL( v1Link.get< std::string >( "exercise.checksum" ), "v1" );

    // replace v1 with v2 and link v2
    const auto tomb1 = UploadAndInstall( "exo", "v2" );
    CheckCounts( install.GetProperties(), 1, 1, 1 );
    BOOST_REQUIRE( install.Find( "exercise", "exo", "v2" ) );
    const auto v2Link = install.LinkExercise( "exo" );
    BOOST_REQUIRE_EQUAL( v2Link.get< std::string >( "exercise.checksum" ), "v2" );

    // replace v2 with v3 and link v3
    const auto tomb2 = UploadAndInstall( "exo", "v3" );
    CheckCounts( install.GetProperties(), 1, 1, 1 );
    const auto v3Link = install.LinkExercise( "exo" );
    BOOST_REQUIRE_EQUAL( v3Link.get< std::string >( "exercise.checksum" ), "v3" );

    Async async( pool );
    // unlink v3, nothing should change
    install.UnlinkItem( async, v3Link );
    BOOST_REQUIRE( install.Find( "exercise", "exo", "v1" ) );
    BOOST_REQUIRE( install.Find( "exercise", "exo", "v2" ) );
    BOOST_REQUIRE( install.Find( "exercise", "exo", "v3" ) );
    CheckCounts( install.GetProperties(), 1, 1, 1 );

    // unlink v1, it gets uninstalled silently
    ExpectItemRemoval( tomb1 );
    install.UnlinkItem( async, v1Link );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v1" ) );
    BOOST_REQUIRE(  install.Find( "exercise", "exo", "v2" ) );
    BOOST_REQUIRE(  install.Find( "exercise", "exo", "v3" ) );
    CheckCounts( install.GetProperties(), 1, 1, 1 );

    // uninstall v3, the current version
    ExpectItemRemoval( tomb2 );
    install.Uninstall( async, tomb2, boost::assign::list_of( 4 ) );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v1" ) );
    BOOST_REQUIRE(  install.Find( "exercise", "exo", "v2" ) );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v3" ) );
    CheckCounts( install.GetProperties(), 1, 1, 0 );

    // unlink v2, it gets uninstalled silently
    ExpectItemRemoval( tomb2 );
    install.UnlinkItem( async, v2Link );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v1" ) );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v2" ) );
    BOOST_REQUIRE( !install.Find( "exercise", "exo", "v3" ) );
    CheckCounts( install.GetProperties(), 1, 1, 0 );
}

BOOST_FIXTURE_TEST_CASE( package_allows_not_replacing_packages, Fixture< false > )
{
    Install( "exo", "v1" );
    UploadAndInstall( "exo", "v2" );
    CheckCounts( install.GetProperties(), 1, 1, 2 );
}

BOOST_FIXTURE_TEST_CASE( package_reloads_metadata, Fixture< true BOOST_PP_COMMA() true > )
{
    // default methods provide empty metadata without checksum and use fs.Checksum
    InstallWith( "ada", "AAAAAAAA", "ter", "BBBBBBBB", "exo", "CCCCCCCC" );

    // don't give checksums and expect them to be read from metadata tag
    T_Paths dst;
    const auto root = install.GetPath();
    const auto odo = GetItemRoot( root, true );
    AddModel( dst, true, odo, "odo", "" );
    const auto tur = GetItemRoot( root, true );
    AddTerrain ( dst, true, tur, "tur", "" );
    const auto fox = GetItemRoot( root, true );
    AddExercise( dst, true, fox, "fox", "", "odo", "tur" );
    const auto gam = GetItemRoot( root, true );
    AddClient( dst, true, gam, "" );
    MOCK_EXPECT( fs.Walk ).once().with( root, false, boost::bind( &MockFileSystem::Apply, &fs, _1, GetItemRoots( dst ) ) );
    // activate metadata caching and check no checksums are done
    MOCK_EXPECT( fs.ReadFile ).once().with( odo / "metadata.tag" ).returns( MakeMetadata( "AAAAAAAA" ) );
    MOCK_EXPECT( fs.ReadFile ).once().with( tur / "metadata.tag" ).returns( MakeMetadata( "BBBBBBBB" ) );
    MOCK_EXPECT( fs.ReadFile ).once().with( fox / "metadata.tag" ).returns( MakeMetadata( "CCCCCCCC" ) );
    MOCK_EXPECT( fs.ReadFile ).once().with( gam / "metadata.tag" ).returns( MakeMetadata( "DDDDDDDD" ) );
    install.Parse();
    mock::verify();
}
