// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "Mocks.h"
#include <host/Package.h>
#include <host/PropertyTree.h>

#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <set>

using namespace host;
using mocks::MockFileSystem;
using mocks::MockPool;

namespace
{
const Path defRoot = "a";
const Path nexRoot = "b";

struct Fixture
{
    Fixture()
        : install( pool, system, defRoot, true )
        , cache  ( pool, system, nexRoot, false )
    {
        // NOTHING
    }

    MockPool pool;
    MockFileSystem system;
    Package install;
    Package cache;
    std::vector< Path > models;
    std::vector< Path > terrains;
    std::vector< Path > exercises;

    std::string GetFileIndex()
    {
        return boost::lexical_cast< std::string >( models.size() + terrains.size() + exercises.size() );
    }

    std::string MakeExerciseData( const std::string& model, const std::string& terrain )
    {
        return "<exercise><model dataset=\"" + model + "\"/><terrain name=\"" + terrain + "\"/></exercise>";
    }

    template< typename T >
    void AddItem( const Path& root, bool ref, T& list, const Path& prefix, const Path& name, const Path& suffix, const std::string& checksum, const std::string& data, const std::string& metadata )
    {
        Path sub = ref ? root / GetFileIndex() : root;
        if( !ref )
        {
            const Path content = root / "content.xml";
            MOCK_EXPECT( system.IsFile ).once().with( content ).returns( true );
            Tree next;
            next.put( "content.name", "some_name" );
            next.put( "content.description", "some_description" );
            next.put( "content.version", "some_version" );
            MOCK_EXPECT( system.ReadFile ).once().with( content ).returns( ToXml( next ).c_str() );
        }
        const Path target = sub / prefix / name / suffix;
        list.push_back( target );
        MOCK_EXPECT( system.GetLastWrite ).with( target ).returns( std::time_t() );
        MOCK_EXPECT( system.ReadFile ).with( target ).returns( data );
        MOCK_EXPECT( system.ReadFile ).with( sub / "metadata.tag" ).returns( metadata );
        MOCK_EXPECT( system.Checksum ).once().with( sub / prefix / name, mock::any ).returns( checksum );
        MOCK_EXPECT( system.Glob ).with( sub / prefix, suffix.filename() ).returns( boost::assign::list_of( target ) );
        if( ref )
            MOCK_EXPECT( system.Glob ).exactly( 2 ).returns( std::vector< Path >() );
    }

    void AddModel( const Path& root, bool ref, const Path& name, const std::string checksum, const std::string& metadata = std::string() )
    {
        AddItem( root, ref, models, "data/models", name, "decisional/decisional.xml", checksum, std::string(), metadata );
    }

    void AddTerrain( const Path& root, bool ref, const Path& name, const std::string checksum, const std::string& metadata = std::string() )
    {
        AddItem( root, ref, terrains, "data/terrains", name, "Terrain.xml", checksum, std::string(), metadata );
    }

    void AddExercise( const Path& root, bool ref, const Path& name, const std::string& checksum, const std::string& model, const std::string& terrain, const std::string& metadata = std::string() )
    {
        AddItem( root, ref, exercises, "exercises", name, "exercise.xml", checksum, MakeExerciseData( model, terrain ), metadata );
    }

    Path Decompose( const Path& path, size_t offset )
    {
        Path rpy;
        for( Path::const_iterator it = path.begin(); it != path.end() && offset; ++it, --offset )
            rpy = rpy / *it;
        return rpy;
    }

    template< typename T >
    std::vector< Path > GetItemRoots( const T& src )
    {
        std::vector< Path > rpy;
        std::set< Path > roots;
        BOOST_FOREACH( const std::vector< Path > vec, src )
            BOOST_FOREACH( const Path& it, vec )
            {
                const Path sub = Decompose( it, 2 );
                if( roots.insert( sub ).second )
                    rpy.push_back( sub );
            }
        return rpy;
    }

    void SetFileSystem( const Path& root )
    {
        const std::vector< Path > paths = GetItemRoots( boost::assign::list_of( models )( terrains )( exercises ) );
        MOCK_EXPECT( system.Walk ).once().with( root, false ).returns( paths );
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

    void AddSomePackages( const Package_ABC& pkg, bool ref )
    {
        const Path root = pkg.GetPath();
        AddModel( root, ref, "ada", "01234567" );
        AddTerrain( root, ref, "egypt", "12345678" );
        AddExercise( root, ref, "shore", "23456789", "ada", "egypt" );

        if( !ref )
            return;

        AddExercise( root, ref, "castle", "34567890", "ada", "avalon" );
        SetFileSystem( root );
    }

    void CheckSomePackages( const Package_ABC& pkg, bool ref )
    {
        const Tree data = pkg.GetProperties();
        BOOST_CHECK_EQUAL( CountItemType( data, "model" ), size_t( 1 ) );
        BOOST_CHECK_EQUAL( CountItemType( data, "terrain" ), size_t( 1 ) );
        BOOST_CHECK_EQUAL( CountItemType( data, "exercise" ), size_t( 1 + !!ref ) );

        const Tree ada = GetItem( data, "model", "ada" );
        BOOST_CHECK_EQUAL( ada.get< std::string >( "checksum" ), "01234567" );

        const Tree egypt = GetItem( data, "terrain", "egypt" );
        BOOST_CHECK_EQUAL( egypt.get< std::string >( "checksum" ), "12345678" );

        const Tree shore = GetItem( data, "exercise", "shore" );
        BOOST_CHECK_EQUAL( shore.get< std::string >( "checksum" ), "23456789" );
        BOOST_CHECK_EQUAL( shore.get< std::string >( "model" ), "ada" );
        BOOST_CHECK_EQUAL( shore.get< std::string >( "terrain" ), "egypt" );

        if( !ref )
            return;

        const Tree castle = GetItem( data, "exercise", "castle" );
        BOOST_CHECK_EQUAL( castle.get< std::string >( "checksum" ), "34567890" );
        BOOST_CHECK_EQUAL( castle.get< std::string >( "model" ), "ada" );
        BOOST_CHECK_EQUAL( castle.get< std::string >( "terrain" ), "avalon" );
        BOOST_CHECK( !castle.get< std::string >( "error" ).empty() );
    }
};
}

BOOST_FIXTURE_TEST_CASE( install_parses, Fixture )
{
    AddSomePackages( install, true );
    install.Parse();
    install.Identify( install );
    CheckSomePackages( install, true );
}

BOOST_FIXTURE_TEST_CASE( cache_parses, Fixture )
{
    AddSomePackages( cache, false );
    cache.Parse();
    cache.Identify( cache );
    CheckSomePackages( cache, false );
}