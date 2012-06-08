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
const Path root = "a";

struct Fixture
{
    Fixture()
        : package( pool, system, root, true )
    {
        // NOTHING
    }

    MockPool pool;
    MockFileSystem system;
    Package package;
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
    void AddItem( T& list, const Path& prefix, const Path& name, const Path& suffix, const std::string& checksum, const std::string& data, const std::string& metadata )
    {
        const Path sub = root / GetFileIndex();
        const Path target = sub / prefix / name / suffix;
        list.push_back( target );
        MOCK_EXPECT( system.GetLastWrite ).with( target ).returns( std::time_t() );
        MOCK_EXPECT( system.ReadFile ).with( target ).returns( data );
        MOCK_EXPECT( system.ReadFile ).with( sub / "metadata.tag" ).returns( metadata );
        MOCK_EXPECT( system.Checksum ).once().with( sub / prefix / name, mock::any ).returns( checksum );
        MOCK_EXPECT( system.Glob ).with( sub / prefix, suffix.filename() ).returns( boost::assign::list_of( target ) );
        MOCK_EXPECT( system.Glob ).exactly( 2 ).returns( std::vector< Path >() );
    }

    void AddModel( const Path& name, const std::string checksum, const std::string& metadata = std::string() )
    {
        AddItem( models, "data/models", name, "decisional/decisional.xml", checksum, std::string(), metadata );
    }

    void AddTerrain( const Path& name, const std::string checksum, const std::string& metadata = std::string() )
    {
        AddItem( terrains, "data/terrains", name, "Terrain.xml", checksum, std::string(), metadata );
    }

    void AddExercise( const Path& name, const std::string& checksum, const std::string& model, const std::string& terrain, const std::string& metadata = std::string() )
    {
        AddItem( exercises, "exercises", name, "exercise.xml", checksum, MakeExerciseData( model, terrain ), metadata );
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

    void SetFileSystem()
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
};
}

BOOST_FIXTURE_TEST_CASE( package_parses, Fixture )
{
    AddModel( "ada", "01234567" );
    AddTerrain( "egypt", "12345678" );
    AddExercise( "shore", "23456789", "ada", "egypt" );
    AddExercise( "castle", "34567890", "ada", "avalon" );
    SetFileSystem();

    package.Parse();
    package.Identify( package );

    const Tree data = package.GetProperties();
    BOOST_CHECK_EQUAL( CountItemType( data, "model" ), size_t( 1 ) );
    BOOST_CHECK_EQUAL( CountItemType( data, "terrain" ), size_t( 1 ) );
    BOOST_CHECK_EQUAL( CountItemType( data, "exercise" ), size_t( 2 ) );

    const Tree ada = GetItem( data, "model", "ada" );
    BOOST_CHECK_EQUAL( ada.get< std::string >( "checksum" ), "01234567" );

    const Tree egypt = GetItem( data, "terrain", "egypt" );
    BOOST_CHECK_EQUAL( egypt.get< std::string >( "checksum" ), "12345678" );

    const Tree shore = GetItem( data, "exercise", "shore" );
    BOOST_CHECK_EQUAL( shore.get< std::string >( "checksum" ), "23456789" );
    BOOST_CHECK_EQUAL( shore.get< std::string >( "model" ), "ada" );
    BOOST_CHECK_EQUAL( shore.get< std::string >( "terrain" ), "egypt" );

    const Tree castle = GetItem( data, "exercise", "castle" );
    BOOST_CHECK_EQUAL( castle.get< std::string >( "checksum" ), "34567890" );
    BOOST_CHECK_EQUAL( castle.get< std::string >( "model" ), "ada" );
    BOOST_CHECK_EQUAL( castle.get< std::string >( "terrain" ), "avalon" );
    BOOST_CHECK( !castle.get< std::string >( "error" ).empty() );
}