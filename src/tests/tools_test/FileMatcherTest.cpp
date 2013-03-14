// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/FileMatcherFactory.h"
#include "tools/FileMatcherRootNode.h"
#include "tools/FileMatcherFileName.h"
#include "tools/FileMatcherNonVersionedSchema.h"
#include <xeumeuleu/xml.hpp>

BOOST_AUTO_TEST_CASE( test_matcher_root_node )
{
    tools::FileMatcherFactory factory;
    xml::xistringstream matcherConf( "<match-if root-node=\"physical\" assign-schema=\"yeah.xsd\"/>" );
    matcherConf >> xml::start( "match-if" );
    boost::shared_ptr< tools::FileMatcher_ABC > matcher = factory.CreateFileMatcher( matcherConf );
    matcherConf >> xml::end;

    xml::xistringstream xisMatching( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                     "<physical xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"whatever.xsd\">"
                                     "  <decisional file=\"Decisionnel.xml\"/>"
                                     "</physical>" );

    tools::Path newSchema;
    bool output = matcher->MatchAndReturnNewSchema( "whatever.xml", xisMatching, newSchema );
    BOOST_CHECK( output );
    BOOST_CHECK_EQUAL( newSchema, "yeah.xsd" );

    xml::xistringstream xisNotMatching( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                        "<other xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"whatever.xsd\">"
                                        "  <decisional file=\"Decisionnel.xml\"/>"
                                        "</other>" );
    output = matcher->MatchAndReturnNewSchema( "whatever.xml", xisNotMatching, newSchema );
    BOOST_CHECK( !output );
    BOOST_CHECK_EQUAL( newSchema, "" );
}

BOOST_AUTO_TEST_CASE( test_matcher_filename )
{
    tools::FileMatcherFactory factory;
    xml::xistringstream matcherConf( "<match-if filename=\"valid-test.xml\" assign-schema=\"yeah.xsd\"/>" );
    matcherConf >> xml::start( "match-if" );
    boost::shared_ptr< tools::FileMatcher_ABC > matcher = factory.CreateFileMatcher( matcherConf );
    matcherConf >> xml::end;

    tools::Path newSchema;
    xml::xistringstream xisEmpty( "<?xml version=\"1.0\"?><empty/>" );
    bool output = matcher->MatchAndReturnNewSchema( "valid-test.xml", xisEmpty, newSchema );
    BOOST_CHECK( output );
    BOOST_CHECK_EQUAL( newSchema, "yeah.xsd" );

    output = matcher->MatchAndReturnNewSchema( "/just/another/valid-test.xml", xisEmpty, newSchema );
    BOOST_CHECK( output );
    BOOST_CHECK_EQUAL( newSchema, "yeah.xsd" );

    output = matcher->MatchAndReturnNewSchema( "notgoo-test.xml", xisEmpty, newSchema );
    BOOST_CHECK( !output );
    BOOST_CHECK_EQUAL( newSchema, "" );

    BOOST_TODO;
    /*output = matcher->MatchAndReturnNewSchema( "invalid-test.xml", xisEmpty, newSchema );
    BOOST_CHECK( !output );
    BOOST_CHECK_EQUAL( newSchema, "" );*/
}

BOOST_AUTO_TEST_CASE( test_matcher_non_versioned_schema )
{
    tools::FileMatcherFactory factory;
    xml::xistringstream matcherConf( "<match-if non-versioned-schema=\"/stupid/schema.xsd\" assign-schema=\"/great/schema.xsd\"/>" );
    matcherConf >> xml::start( "match-if" );
    boost::shared_ptr< tools::FileMatcher_ABC > matcher = factory.CreateFileMatcher( matcherConf );
    matcherConf >> xml::end;

    xml::xistringstream xisMatching( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                     "<physical xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"/stupid/schema.xsd\">"
                                     "  <decisional file=\"Decisionnel.xml\"/>"
                                     "</physical>" );

    tools::Path newSchema;
    bool output = matcher->MatchAndReturnNewSchema( "whatever.xml", xisMatching, newSchema );
    BOOST_CHECK( output );
    BOOST_CHECK_EQUAL( newSchema, "/great/schema.xsd" );

    xml::xistringstream xisNotMatching( "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                                        "<other xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"whatever.xsd\">"
                                        "  <decisional file=\"Decisionnel.xml\"/>"
                                        "</other>" );
    output = matcher->MatchAndReturnNewSchema( "whatever.xml", xisNotMatching, newSchema );
    BOOST_CHECK( !output );
    BOOST_CHECK_EQUAL( newSchema, "" );
}

