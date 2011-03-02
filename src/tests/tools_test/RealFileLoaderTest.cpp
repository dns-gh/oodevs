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
#include "tools/RealFileLoader.h"
#include "tools/SchemaVersionExtractor.h"
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

namespace
{
    struct Fixture
    {
        Fixture()
            : migrations_     ( BOOST_RESOLVE( "testFileMigration/migrations.xml" ) )
            , emptyMigrations_( BOOST_RESOLVE( "testFileMigration/empty_migrations.xml" ) )
        {
            boost::filesystem::path outputDir2 = tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" );
            boost::filesystem::remove_all( outputDir2 );
            boost::filesystem::create_directories( outputDir2 );
            boost::filesystem::create_directories( outputDir2 / "1.0" );
            boost::filesystem::copy_file( BOOST_RESOLVE( "testFileMigration/test-1.3.xsl" ), outputDir2 / "test-1.3.xsl" );
            boost::filesystem::copy_file( BOOST_RESOLVE( "testFileMigration/test-1.4.xsl" ), outputDir2 / "test-1.4.xsl" );
            boost::filesystem::copy_file( BOOST_RESOLVE( "testFileMigration/1.0/schema.xsd" ), outputDir2 / "1.0/schema.xsd" );
            boost::filesystem::copy_file( BOOST_RESOLVE( "testFileMigration/schema.xsd" ), outputDir2 / "schema.xsd" );
        }

        ~Fixture()
        {
            boost::filesystem::remove_all( tools::GeneralConfig::BuildResourceChildFile( "testFileMigration" ) );
        }

        xml::xifstream migrations_;
        xml::xifstream emptyMigrations_;
    };
}

BOOST_FIXTURE_TEST_CASE( test_invalid_migrations_file_missing_version, Fixture )
{
    tools::SchemaVersionExtractor ve;
    xml::xifstream invalidMigrations( BOOST_RESOLVE( "testFileMigration/invalid_migrations.xml" ) );
    BOOST_CHECK_THROW( tools::RealFileLoader loader( invalidMigrations, ve ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_schema, Fixture )
{
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( "testFileMigration/input-1.0.xml" ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_schema_throws, Fixture )
{
    const std::string inputFile( "testFileMigration/input_invalid_schema.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyInvalidXml ).once().with( BOOST_RESOLVE( inputFile ), mock::any ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_schema_nothrows, Fixture )
{
    const std::string inputFile( "testFileMigration/input_invalid_schema.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyInvalidXml ).once().with( BOOST_RESOLVE( inputFile ), mock::any ).returns( true );
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_signature_throws, Fixture )
{
    const std::string inputFile( "testFileMigration/input_invalid_signature.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifySignatureError ).once().with( BOOST_RESOLVE( inputFile ), tools::eXmlCrc32SignatureError_Invalid ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_signature_nothrows, Fixture )
{
    const std::string inputFile( "testFileMigration/input_invalid_signature.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifySignatureError ).once().with( BOOST_RESOLVE( inputFile ), tools::eXmlCrc32SignatureError_Invalid ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_missing_xml_signature_throws, Fixture )
{
    const std::string inputFile( "testFileMigration/input_missing_signature.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifySignatureError ).once().with( BOOST_RESOLVE( inputFile ), tools::eXmlCrc32SignatureError_NotSigned ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_missing_xml_signature_nothrows, Fixture )
{
    const std::string inputFile( "testFileMigration/input_missing_signature.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifySignatureError ).once().with( BOOST_RESOLVE( inputFile ), tools::eXmlCrc32SignatureError_NotSigned ).returns( false );
    BOOST_CHECK_THROW( loader.LoadFile( BOOST_RESOLVE( inputFile ), observer ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( test_invalid_xml_schema_and_signature_nothrows, Fixture )
{
    const std::string inputFile( "testFileMigration/input_invalid_schema_and_signature.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( emptyMigrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyInvalidXml ).once().with( BOOST_RESOLVE( inputFile ), mock::any ).returns( true );
    MOCK_EXPECT( observer, NotifySignatureError ).once().with( BOOST_RESOLVE( inputFile ), tools::eXmlCrc32SignatureError_Invalid ).returns( true );
    loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_schema_migration, Fixture )
{
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( migrations_, ve );

    const std::string inputFile( BOOST_RESOLVE( "testFileMigration/input-1.0.xml" ) );

    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );

    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
    xml::xostringstream actual;
    actual << *inputStream;

    xml::xifstream tmp( BOOST_RESOLVE( "testFileMigration/input-1.4.xml" ) );
    xml::xostringstream expected;
    expected << tmp;
    BOOST_CHECK_XML_EQUAL( actual.str(), expected.str() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_no_legacy_assignment, Fixture )
{
    xml::xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_no_legacy_assignment.xml" ) );
    const std::string inputFile( "testFileMigration/input_valid_no_version_in_schema.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( migrations, ve );
    MockRealFileLoaderObserver observer;
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_schema_no_legacy_assignment, Fixture )
{
    xml::xifstream migrations( BOOST_RESOLVE( "testFileMigration/migrations_no_legacy_assignment.xml" ) );
    const std::string inputFile( "testFileMigration/input_valid_no_schema.xml" );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( migrations, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyNoXmlSchemaSpecified ).once().with( BOOST_RESOLVE( inputFile ) );
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( BOOST_RESOLVE( inputFile ), observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_schema_legacy_assignment, Fixture )
{
    const std::string inputFile( BOOST_RESOLVE( "testFileMigration/input_valid_no_schema.xml" ) );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( migrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

BOOST_FIXTURE_TEST_CASE( test_valid_xml_no_version_in_schema_legacy_assignment, Fixture )
{
    const std::string inputFile( BOOST_RESOLVE( "testFileMigration/input_valid_no_version_in_schema.xml" ) );
    tools::SchemaVersionExtractor ve;
    tools::RealFileLoader loader( migrations_, ve );
    MockRealFileLoaderObserver observer;
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.0", "1.1" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.1", "1.2" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.2", "1.3" );
    MOCK_EXPECT( observer, NotifyFileMigrated ).once().with( inputFile, "1.3", "1.4" );
    std::auto_ptr< xml::xistream > inputStream = loader.LoadFile( inputFile, observer );
    BOOST_REQUIRE( inputStream.get() );
}

// test migration failed
// test
