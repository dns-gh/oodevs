// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "tools/SchemaVersionExtractor.h"

BOOST_AUTO_TEST_CASE( test_schema_version_extractor )
{
    tools::SchemaVersionExtractor ve;
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "1.0.1" ), "1.0.1" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "1.0.1/schema/validation.xml" ), "1.0.1" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "schema/1.0.1/validation.xml" ), "1.0.1" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "schema/validation.xml" ), "" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "1.2.3/schema/1.0.1/validation.xml" ), "1.2.3" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "1/schema/validation.xml" ), "1" );
    BOOST_CHECK_EQUAL( ve.ExtractVersion( "schema/2/validation.xml" ), "2" );
    //BOOST_CHECK_EQUAL( ve.ExtractVersion( "schema/validation1.xml" ), "" );
}
