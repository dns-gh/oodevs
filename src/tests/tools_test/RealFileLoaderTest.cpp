// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "MockRealFileLoaderObserver.h"
#include "tools/FileWrapper.h"
#include "tools/RealFileLoader.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    struct Fixture
    {
        Fixture()
            : //migrations_     ( BOOST_RESOLVE( "testFileMigration/migrations_no_default_assignment.xml" ) )
             emptyMigrations_( BOOST_RESOLVE( "testFileMigration/empty_migrations.xml" ) )
        {
            tools::Path outputDir = tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" );
            outputDir.RemoveAll();
            ( outputDir / "1.0" ).CreateDirectories();
            BOOST_RESOLVE( "testFileMigration/test-1.3.xsl" ).Copy( outputDir / "test-1.3.xsl" );
            BOOST_RESOLVE( "testFileMigration/test-1.4.xsl" ).Copy( outputDir / "test-1.4.xsl" );
            BOOST_RESOLVE( "testFileMigration/1.0/schema.xsd" ).Copy( outputDir / "1.0/schema.xsd" );
            BOOST_RESOLVE( "testFileMigration/schema.xsd" ).Copy( outputDir / "schema.xsd" );
            BOOST_RESOLVE( "testFileMigration/input-1.0.xml" ).Copy( outputDir / "input-1.0.xml" );
        }

        ~Fixture()
        {
            tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" ).RemoveAll();
        }

        //tools::Xifstream migrations_;
        tools::Xifstream emptyMigrations_;
    };
}

BOOST_AUTO_TEST_CASE( test_schema_version_extractor )
{
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "2.0.1" ), "1.0.1" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "1.0.1/schema/validation.xml" ), "1.0.1" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "schema/1.0.1/validation.xml" ), "1.0.1" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "schema/validation.xml" ), "" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "1.2.3/schema/1.0.1/validation.xml" ), "1.2.3" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "1/schema/validation.xml" ), "1" );
    BOOST_CHECK_EQUAL( tools::ExtractSchemaVersion( "schema/2/validation.xml" ), "2" );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_migrations_file_missing_version, Fixture )
{
    tools::Xifstream invalidMigrations( BOOST_RESOLVE( "testFileMigration/invalid_migrations.xml" ) );
    BOOST_CHECK_THROW( tools::RealFileLoader loader( invalidMigrations ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_schema, Fixture )
{
    tools::RealFileLoader loader( emptyMigrations_ );
    MockRealFileLoaderObserver observer;
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( "testFileMigration/input-1.0.xml" ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_schema_throws, Fixture )
{
    const tools::Path inputFile = "testFileMigration/input_invalid_schema.xml";
    tools::RealFileLoader loader( emptyMigrations_ );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyInvalidXml ).once().with( BOOST_RESOLVE( inputFile ), mock::any ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_schema_nothrows, Fixture )
{
    const tools::Path inputFile = "testFileMigration/input_invalid_schema.xml";
    tools::RealFileLoader loader( emptyMigrations_ );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyInvalidXml ).once().with( BOOST_RESOLVE( inputFile ), mock::any ).returns( true );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_no_version_in_schema, Fixture )
{
    const tools::Path inputFile = "testFileMigration/input_invalid_no_version_in_schema.xml";
    tools::RealFileLoader loader( emptyMigrations_ );
    MockRealFileLoaderObserver observer;
    // NotifyInvalidXml won't be called: no version in initial schema = don't check
    loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_schema_migration, Fixture )
{
    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations.xml" ) );
    tools::RealFileLoader loader( migrations );

    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input-1.0.xml" );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );

    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
    xml::xostringstream actual;
    actual << *inputStream;

    tools::Xifstream tmp( BOOST_RESOLVE( "testFileMigration/input-1.4.xml" ) );
    xml::xostringstream expected;
    expected << tmp;
    BOOST_CHECK_XML_EQUAL( actual.str(), expected.str() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_no_default_assignment, Fixture )
{
    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_no_default_assignment.xml" ) );
    const tools::Path inputFile = "testFileMigration/input_valid_no_version_in_schema.xml";
    tools::RealFileLoader loader( migrations );
    MockRealFileLoaderObserver observer;
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_schema_no_default_assignment, Fixture )
{
    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_no_default_assignment.xml" ) );

    const tools::Path inputFile = "testFileMigration/input_valid_no_schema.xml";
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyNoXmlSchemaSpecified ).once().with( BOOST_RESOLVE( inputFile ) );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_schema_default_assignment_rootnode, Fixture )
{
    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input_valid_no_schema.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_default_rootnode.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_default_assignment_rootnode, Fixture )
{
    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input_valid_no_version_in_schema.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_default_rootnode.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_schema_default_assignment_filename, Fixture )
{
    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input_valid_no_schema.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_default_filename.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_default_assignment_filename, Fixture )
{
    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input_valid_no_version_in_schema.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_default_filename.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_default_assignment_non_versioned_schema, Fixture )
{
    const tools::Path inputFile = BOOST_RESOLVE( "testFileMigration/input_valid_no_version_in_schema.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_default_schema.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_added_file, Fixture )
{
    const tools::Path inputFile = tools::GeneralConfig::BuildResourceChildFile( "testFileMigration/input-1.0.xml" );

    tools::Xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_added_files.xml" ) );
    tools::RealFileLoader loader( migrations );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::unique_ptr< xml::xistream > inputStream = loader.LoadFile( "physical/test.xml", observer );
    BOOST_REQUIRE( inputStream.get() );

    BOOST_CHECK_THROW( loader.LoadFile( "test.xml", observer ), std::exception );
    BOOST_REQUIRE( inputStream.get() );

    BOOST_CHECK_THROW( loader.LoadFile( "/resource/test.xml", observer ), std::exception );
    BOOST_REQUIRE( inputStream.get() );

    //BOOST_CHECK_THROW( loader.LoadFile( "resourcephysical/test.xml", observer ), std::exception );
    //BOOST_REQUIRE( inputStream.get() );
    BOOST_TODO;

    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    inputStream = loader.LoadFile( "/physical/test.xml", observer );
    BOOST_REQUIRE( inputStream.get() );

    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer.NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    inputStream = loader.LoadFile( "/whatever/resource/blabla/physical/test.xml", observer );
    BOOST_REQUIRE( inputStream.get() );
}

// test migration failed
// test
