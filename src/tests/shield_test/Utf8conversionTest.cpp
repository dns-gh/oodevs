// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "Protocol.h"
#include "shield/Utf8Converter.h"
#include <Windows.h>

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_simple_CP1252toUTF8
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_simple_CP1252toUTF8 )
{
    SimpleMessage message;
    message.set_truk( "�Hola! �A donde vas?� �tre loin de ma for�t, o� les �ufs Magic� � 1� abondent �Mon �ne e�t �t� si pr�s de ton �le�" );
    Utf8Converter::ConvertCP1252StringsToUtf8( message );
    BOOST_CHECK( message.truk() == "¡Hola! ¿A donde vas?… Être loin de ma forêt, où les œufs Magic™ à 1€ abondent …Mon âne eût été si près de ton île…" );
    BOOST_CHECK( !message.has_choz() );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_simple_UTF8toCP1252
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_simple_UTF8toCP1252 )
{
    SimpleMessage message;
    message.set_truk( "¡Hola! ¿A donde vas?… Être loin de ma forêt, où les œufs Magic™ à 1€ abondent …Mon âne eût été si près de ton île…" );
    Utf8Converter::ConvertUtf8StringsToCP1252( message );
    BOOST_CHECK( message.truk() == "�Hola! �A donde vas?� �tre loin de ma for�t, o� les �ufs Magic� � 1� abondent �Mon �ne e�t �t� si pr�s de ton �le�" );
    BOOST_CHECK( !message.has_choz() );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_simple_both_way
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_simple_double_conversion )
{
    SimpleMessage message;
    message.set_truk( "�Hola! �A donde vas?� �tre loin de ma for�t, o� les �ufs Magic� � 1� abondent �Mon �ne e�t �t� si pr�s de ton �le�" );
    Utf8Converter::ConvertCP1252StringsToUtf8( message );
    Utf8Converter::ConvertUtf8StringsToCP1252( message );
    BOOST_CHECK( message.truk() == "�Hola! �A donde vas?� �tre loin de ma for�t, o� les �ufs Magic� � 1� abondent �Mon �ne e�t �t� si pr�s de ton �le�" );
    BOOST_CHECK( !message.has_choz() );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_composite_message_CP1252toUTF8
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_composite_message_CP1252toUTF8 )
{
    CompositeMessage message;
    message.set_truk( "for�t" );
    message.mutable_sub_truk()->set_stuff( "�ufs Magic�" );
    Utf8Converter::ConvertCP1252StringsToUtf8( message );
    BOOST_CHECK( message.truk() == "forêt" );
    BOOST_CHECK( message.has_sub_truk() && message.sub_truk().stuff() == "œufs Magic™" );
    BOOST_CHECK( !message.has_optional_machin() );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_more_complex_message_both_way
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_more_complex_message_both_way )
{
    MoreComplexMessage message;
    message.set_truk( "for�t" );
    message.add_choses( "1�" );
    message.add_choses( "�ne" );
    message.add_choses( "�t�" );
    message.mutable_sub_truk()->set_stuff( "�ufs Magic�" );
    Utf8Converter::ConvertCP1252StringsToUtf8( message );
    BOOST_CHECK( message.truk() == "forêt" );
    BOOST_CHECK( message.choses( 0 ) == "1€" );
    BOOST_CHECK( message.choses( 1 ) == "âne" );
    BOOST_CHECK( message.choses( 2 ) == "été" );
    BOOST_CHECK( message.has_sub_truk() && message.sub_truk().stuff() == "œufs Magic™" );
    BOOST_CHECK( !message.has_optional_machin() );
    //
    Utf8Converter::ConvertUtf8StringsToCP1252( message );
    BOOST_CHECK( message.truk() == "for�t" );
    BOOST_CHECK( message.choses( 0 ) == "1�" );
    BOOST_CHECK( message.choses( 1 ) == "�ne" );
    BOOST_CHECK( message.choses( 2 ) == "�t�" );
    BOOST_CHECK( message.has_sub_truk() && message.sub_truk().stuff() == "�ufs Magic�" );
    BOOST_CHECK( !message.has_optional_machin() );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_really_complex_message_both_way
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_really_complex_message_both_way )
{
    ReallyComplexMessage message;
    message.set_truk( "for�t" );
    message.add_choses( "1�" );
    message.add_choses( "�ne" );
    message.add_choses( "�t�" );
    message.add_sub_bidules()->set_stuff( "�ufs Magic�" );
    message.set_optional_machin( "�le" );
    message.mutable_sub_chouette()->add_sub_machins()->set_stuff( "vo�te" );
    message.mutable_sub_chouette()->add_sub_machins()->set_stuff( "colima�on" );
    message.mutable_sub_chouette()->set_bidule( "pr�s" );
    Utf8Converter::ConvertCP1252StringsToUtf8( message );
    BOOST_CHECK( message.truk() == "forêt" );
    BOOST_CHECK( message.choses( 0 ) == "1€" );
    BOOST_CHECK( message.choses( 1 ) == "âne" );
    BOOST_CHECK( message.choses( 2 ) == "été" );
    BOOST_CHECK( message.sub_bidules( 0 ).stuff() == "œufs Magic™" );
    BOOST_CHECK( message.optional_machin() == "île" );
    BOOST_CHECK( message.sub_chouette().sub_machins( 0 ).stuff() == "voûte" );
    BOOST_CHECK( message.sub_chouette().sub_machins( 1 ).stuff() == "colimaçon" );
    BOOST_CHECK( message.sub_chouette().bidule() == "près" );
    //
    Utf8Converter::ConvertUtf8StringsToCP1252( message );
    BOOST_CHECK( message.truk() == "for�t" );
    BOOST_CHECK( message.choses( 0 ) == "1�" );
    BOOST_CHECK( message.choses( 1 ) == "�ne" );
    BOOST_CHECK( message.choses( 2 ) == "�t�" );
    BOOST_CHECK( message.sub_bidules( 0 ).stuff() == "�ufs Magic�" );
    BOOST_CHECK( message.optional_machin() == "�le" );
    BOOST_CHECK( message.sub_chouette().sub_machins( 0 ).stuff() == "vo�te" );
    BOOST_CHECK( message.sub_chouette().sub_machins( 1 ).stuff() == "colima�on" );
    BOOST_CHECK( message.sub_chouette().bidule() == "pr�s" );
}

// -----------------------------------------------------------------------------
// Name: Utf8conversionTest_performance_test
// Created: RPD 2011-09-06
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Utf8conversionTest_performance_test )
{
    ReallyComplexMessage message;
    message.set_truk( "�Hola! �A donde vas?� �tre loin de ma for�t, o� les �ufs Magic� � 1� abondent �Mon �ne e�t �t� si pr�s de ton �le�" );
    message.add_choses( "1�" );
    message.add_choses( "�ne" );
    message.add_choses( "�t�" );
    message.add_sub_bidules()->set_stuff( "�ufs Magic�" );
    message.set_optional_machin( "�le" );
    message.mutable_sub_chouette()->add_sub_machins()->set_stuff( "vo�te" );
    message.mutable_sub_chouette()->add_sub_machins()->set_stuff( "colima�on" );
    message.mutable_sub_chouette()->set_bidule( "pr�s" );
    //long before = GetTickCount();
    for ( int i = 0; i < 10000; ++i )
    {
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
        Utf8Converter::ConvertUtf8StringsToCP1252( message );
    }
    //printf ( "Duration for 20.000 CP1252<->utf8 message conversions was %ul milliseconds.", GetTickCount() - before );
}
