// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/Package.h"
#include "runtime/PropertyTree.h"
#include "runtime/Async.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <set>

#include "MockFileSystem.h"
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

struct Fixture
{
    Fixture()
        : idx    ( 0 )
        , install( pool, system, GetFileIndex(), true )
        , cache  ( pool, system, GetFileIndex(), false )
    {
        // NOTHING
    }

    size_t idx;
    MockPool pool;
    MockFileSystem system;
    Package install;
    Package cache;

    std::string GetFileIndex()
    {
        return boost::lexical_cast< std::string >( idx++ );
    }

    std::string MakeExerciseData( const std::string& model, const std::string& terrain ) const
    {
        return "<exercise><model dataset=\"" + model + "\"/><terrain name=\"" + terrain + "\"/></exercise>";
    }

    void AddItem( T_Paths& dst, bool ref, const Path& root, const Path& prefix, const Path& name, const Path& suffix, const std::string& checksum, const std::string& contents )
    {
        const Path data = root / prefix;
        const Path file = data / name / suffix;
        MOCK_EXPECT( system.Glob ).once().with( data, suffix.filename(), boost::bind( &MockFileSystem::Apply, &system, _1, boost::assign::list_of( file ) ) );
        if( ref )
            MOCK_EXPECT( system.Glob ).exactly( 2 ).with( boost::bind( &BeginWith, root, _1 ), mock::any, mock::any );
        MOCK_EXPECT( system.GetLastWrite ).with( file ).returns( std::time_t() );
        MOCK_EXPECT( system.ReadFile ).with( file ).returns( contents );
        MOCK_EXPECT( system.ReadFile ).with( root / "metadata.tag" ).returns( "" );
        MOCK_EXPECT( system.Checksum ).with( data / name, mock::any, mock::any ).returns( checksum );
        dst.push_back( file );
    }

    void AddModel( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum )
    {
        AddItem( dst, ref, root, "data/models", name, "decisional/decisional.xml", checksum, "" );
    }

    void AddTerrain( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum )
    {
        AddItem( dst, ref, root, "data/terrains", name, "Terrain.xml", checksum, "" );
    }

    void AddExercise( T_Paths& dst, bool ref, const Path& root, const Path& name, const std::string& checksum, const std::string& model, const std::string& terrain )
    {
        AddItem( dst, ref, root, "exercises", name, "exercise.xml", checksum, MakeExerciseData( model, terrain ) );
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
        MOCK_EXPECT( system.IsFile ).with( content ).returns( true );
        Tree next;
        next.put( "content.name", name );
        next.put( "content.description", description );
        next.put( "content.version", version );
        MOCK_EXPECT( system.ReadFile ).with( content ).returns( ToXml( next ).c_str() );
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
        MOCK_EXPECT( system.Walk ).once().with( root, false, boost::bind( &MockFileSystem::Apply, &system, _1, GetItemRoots( dst ) ) );
    }

    size_t CountItemType( const Tree& src, const std::string& type )
    {
        size_t count = 0;
        const Tree& sub = src.find( "items" )->second;
        for( Tree::const_iterator it = sub.begin(); it != sub.end(); ++it )
            count += it->second.get< std::string >( "type" ) == type;
        return count;
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

    void CheckCounts( const Tree& data, size_t models, size_t terrains, size_t exercises )
    {
        BOOST_CHECK_EQUAL( CountItemType( data, "model" ), models );
        BOOST_CHECK_EQUAL( CountItemType( data, "terrain" ), terrains );
        BOOST_CHECK_EQUAL( CountItemType( data, "exercise" ), exercises );
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

    void InstallPackage( const std::string& type )
    {
        const Path root = install.GetPath();
        const Path sub = root / GetFileIndex();
        MOCK_EXPECT( system.MakePaths ).with( root );
        MOCK_EXPECT( system.MakeAnyPath ).once().with( root ).returns( sub );
        MOCK_EXPECT( system.MakePaths ).once().with( boost::bind( &BeginWith, sub, _1 ) );
        MOCK_EXPECT( system.CopyDirectory ).once();
        MOCK_EXPECT( system.WriteFile ).once().with( boost::bind( &EndWith, "metadata.tag", _1 ), mock::any ).returns( true );
        T_Paths dst;
        size_t idx;
        if( type == "model" )
        {
            AddModel( dst, true, sub, "ada", "01234567" );
            idx = 0;
        }
        else if( type == "terrain" )
        {
            AddTerrain( dst, true, sub, "egypt", "12345678" );
            idx = 1;
        }
        else if( type == "exercise" )
        {
            AddExercise( dst, true, sub, "shore", "23456789", "ada", "egypt" );
            idx = 2;
        }
        Async async( pool );
        install.Install( async, root / GetFileIndex(), cache, boost::assign::list_of( idx ) );
    }

    void InstallSomePackage()
    {
        AddCheckPackages( cache, false );
        InstallPackage( "model" );
        InstallPackage( "terrain" );
        InstallPackage( "exercise" );

        const Tree data = install.GetProperties();
        CheckCounts( data, 1, 1, 1 );
        CheckItem( data, "model", "ada", "01234567" );
    }

    void ExpectItemRemoval( const Path& root )
    {
        const Path sub = root / GetFileIndex();
        MOCK_EXPECT( system.MakeAnyPath ).once().with( root ).returns( sub );
        MOCK_EXPECT( system.Rename ).once().with( boost::bind( &BeginWith, install.GetPath(), _1 ), boost::bind( &BeginWith, sub, _1 ) ).returns( true );
        MOCK_EXPECT( system.Remove ).with( sub ).returns( true );
    }
};
}

BOOST_FIXTURE_TEST_CASE( install_parses, Fixture )
{
    AddCheckPackages( install, true );
}

BOOST_FIXTURE_TEST_CASE( cache_parses, Fixture )
{
    AddCheckPackages( cache, false );
}

BOOST_FIXTURE_TEST_CASE( package_installs, Fixture )
{
    InstallSomePackage();
}

BOOST_FIXTURE_TEST_CASE( package_uninstalls, Fixture )
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

BOOST_FIXTURE_TEST_CASE( package_links, Fixture )
{
    InstallSomePackage();

    const Tree link = install.LinkExercise( "shore" );
    const Tree other = install.LinkItem( link );

    const Path dst = GetFileIndex();
    Async async( pool );
    MOCK_EXPECT( system.WriteFile ).once().with( boost::bind( &EndWith, "metadata.tag", _1 ), mock::any ).returns( true );
    install.Uninstall( async, dst, boost::assign::list_of( 0 ) );
    const Tree data = install.GetProperties();
    CheckCounts( data, 0, 1, 1 );
    mock::verify();

    install.UnlinkItem( async, other );
    mock::verify();

    ExpectItemRemoval( dst );
    install.UnlinkItem( async, link );
}

BOOST_FIXTURE_TEST_CASE( package_reinstall_silently, Fixture )
{
    InstallSomePackage();
    install.LinkExercise( "shore" );

    Async async( pool );
    MOCK_EXPECT( system.WriteFile ).once().with( boost::bind( &EndWith, "metadata.tag", _1 ), mock::any ).returns( true );
    install.Uninstall( async, GetFileIndex(), boost::assign::list_of( 2 ) );
    mock::verify();

    MOCK_EXPECT( system.WriteFile ).once().with( boost::bind( &EndWith, "metadata.tag", _1 ), mock::any ).returns( true );
    install.Install( async, install.GetPath() / GetFileIndex(), cache, boost::assign::list_of( 2 ) );
}