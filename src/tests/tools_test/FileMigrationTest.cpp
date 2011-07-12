// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/FileMigration.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>
#include <iostream>

namespace
{
    struct Fixture
    {
        Fixture()
        {
            boost::filesystem::path outputDir = tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" );
            boost::filesystem::remove_all( outputDir );
            boost::filesystem::create_directories( outputDir );
            boost::filesystem::copy_file( BOOST_RESOLVE( "testFileMigration/test-1.3.xsl" ), outputDir / "test-1.3.xsl" );
        }

        ~Fixture()
        {
            boost::filesystem::remove_all( tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" ) );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( test_single_migration, Fixture )
{
    xml::xistringstream xisMigrations( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                        "<migration from=\"1.2\" to=\"1.3\">"
                                        "  <upgrade schema=\"testFileMigration/1.2/schema.xsd\" apply=\"testFileMigration/test-1.3.xsl\"/>"
                                        "</migration>" );

    xisMigrations.start( "migration" );
    tools::FileMigration fileMigration( xisMigrations );

    std::auto_ptr< xml::xistream > inputStream = fileMigration.UpgradeFile( std::auto_ptr< xml::xistream >( new xml::xifstream( BOOST_RESOLVE( "testFileMigration/input-1.2.xml" ) ) ), "testFileMigration/1.2/schema.xsd" );
    BOOST_REQUIRE( inputStream.get() );
    xml::xostringstream actual;
    actual << *inputStream;

    xml::xifstream tmp( BOOST_RESOLVE( "testFileMigration/input-1.3.xml" ) );
    xml::xostringstream expected;
    expected << tmp;
    BOOST_CHECK_XML_EQUAL( actual.str(), expected.str() );
}

BOOST_FIXTURE_TEST_CASE( test_single_migration_not_applicable, Fixture )
{
    xml::xistringstream xisMigrations( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                        "<migration from=\"1.2\" to=\"1.3\">"
                                        "  <upgrade schema=\"testFileMigration/1.2/schema.xsd\" apply=\"testFileMigration/test-1.3.xsl\"/>"
                                        "</migration>" );

    xisMigrations.start( "migration" );
    tools::FileMigration fileMigration( xisMigrations );

    std::auto_ptr< xml::xistream > inputStream = fileMigration.UpgradeFile( std::auto_ptr< xml::xistream >( new xml::xifstream( BOOST_RESOLVE( "testFileMigration/input-1.2.xml" ) ) ), "testFileMigration/1.2/OTHER_SCHEMA.xsd" );
    BOOST_REQUIRE( inputStream.get() );
    xml::xostringstream actual;
    actual << *inputStream;

    // File not changed
    xml::xifstream tmp( BOOST_RESOLVE( "testFileMigration/input-1.2.xml" ) );
    xml::xostringstream expected;
    expected << tmp;

    BOOST_CHECK_XML_EQUAL( actual.str(), expected.str() );
}

BOOST_FIXTURE_TEST_CASE( test_migration_invalid_xsl, Fixture )
{
    xml::xistringstream xis( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                             "<migration from=\"1.2\" to=\"1.3\">"
                             "  <upgrade schema=\"testFileMigration/1.2/schema.xsd\" apply=\"testFileMigration/INVALID.xsl\"/>"
                             "</migration>" );

    xis.start( "migration" );
    BOOST_CHECK_THROW( tools::FileMigration fileMigration( xis ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_migration_several_migrations_for_schema, Fixture )
{
    xml::xistringstream xis( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                             "<migration from=\"1.2\" to=\"1.3\">"
                             "  <upgrade schema=\"testFileMigration/1.2/schema.xsd\" apply=\"testFileMigration/INVALID.xsl\"/>"
                             "  <upgrade schema=\"testFileMigration/1.2/schema.xsd\" apply=\"testFileMigration/INVALID.xsl\"/>"
                             "</migration>" );
    xis.start( "migration" );
    BOOST_CHECK_THROW( tools::FileMigration fileMigration( xis ), std::exception );
}
