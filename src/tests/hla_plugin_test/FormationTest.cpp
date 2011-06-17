// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/Formation.h"
#include "SerializationFixture.h"

using namespace plugins::hla;

BOOST_FIXTURE_TEST_CASE( column_formation_on_road, SerializationFixture )
{
    const Formation formation( true );
    ::hla::Deserializer deserializer = Serialize( formation, sizeof( int32 ) );
    const int32 column = 5;
    BOOST_CHECK_EQUAL( column, Read< int32 >( deserializer ) );
}

BOOST_FIXTURE_TEST_CASE( assembly_formation_on_road, SerializationFixture )
{
    const Formation formation( false );
    ::hla::Deserializer deserializer = Serialize( formation, sizeof( int32 ) );
    const int32 assembly = 1;
    BOOST_CHECK_EQUAL( assembly, Read< int32 >( deserializer ) );
}
