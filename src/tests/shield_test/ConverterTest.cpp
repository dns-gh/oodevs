// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "Tools.h"
#include "shield/Utf8Converter.h"

using namespace shield;

BOOST_FIXTURE_TEST_CASE( unknown_message_is_NOT_logged_then_dropped, Fixture )
{
    sword::SimToClient msg;
    msg.mutable_message()->mutable_burning_cell_request_ack()->set_error_code( sword::BurningCellRequestAck::no_error );
    // NO: MOCK_EXPECT( listener, Info ).once().with( "Shield converter dropping unknown 'sword.SimToClient.Content.burning_cell_request_ack' message" );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( utf8_strings_are_converted_to_ansi, Fixture )
{
    std::string utf8string ( "Â¡Hola! Â¿A donde vas?â€¦ ÃŠtre loin de ma forÃªt, oÃ¹ les Å“ufs Magicâ„¢ Ã  1â‚¬ abondent â€¦Mon Ã¢ne eÃ»t Ã©tÃ© si prÃ¨s de ton Ã®leâ€¦" );
    std::string ansiString;
    Utf8Converter::Utf8ToCP1252( utf8string, ansiString );
    BOOST_CHECK( ansiString == "¡Hola! ¿A donde vas?… Être loin de ma forêt, où les œufs Magic™ à 1€ abondent …Mon âne eût été si près de ton île…" );
}

BOOST_FIXTURE_TEST_CASE( ansi_strings_are_converted_to_utf8, Fixture )
{
    std::string ansiString ( "¡Hola! ¿A donde vas?… Être loin de ma forêt, où les œufs Magic™ à 1€ abondent …Mon âne eût été si près de ton île…" );
    std::string utf8string;
    Utf8Converter::CP1252ToUtf8( ansiString, utf8string );
    BOOST_CHECK( utf8string == "Â¡Hola! Â¿A donde vas?â€¦ ÃŠtre loin de ma forÃªt, oÃ¹ les Å“ufs Magicâ„¢ Ã  1â‚¬ abondent â€¦Mon Ã¢ne eÃ»t Ã©tÃ© si prÃ¨s de ton Ã®leâ€¦" );
}