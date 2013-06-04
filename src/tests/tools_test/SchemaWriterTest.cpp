// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/SchemaWriter.h"
#include "tools/VersionHelper.h"
#include <xeumeuleu/xml.hpp>

BOOST_AUTO_TEST_CASE( test_schema_writer )
{
    tools::SchemaWriter schemaWriter;

    xml::xostringstream xos;
    xos << xml::start( "test" );
    schemaWriter.WriteExerciseSchema( xos, "test" );
    xos << xml::end;
    xos.str();

    xml::xistringstream xisTmp( "<test xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" model-version=\"" + std::string( tools::AppModelVersion() ) + "\" xsi:noNamespaceSchemaLocation=\"schemas/" + std::string( tools::AppMajorVersion() ) + "/exercise/test.xsd\"/>" );
    xml::xostringstream expected;
    expected << xisTmp;
    BOOST_CHECK_XML_EQUAL( expected.str(), xos.str() );
}
