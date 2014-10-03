// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "runtime_test.h"
#include <runtime/Utf8.h>
#include <boost/filesystem/path.hpp>

using namespace runtime;

namespace
{
    #define COUNT_OF( X ) ( sizeof( X ) / sizeof*( X ) )

    // tamil poetry converted with http://macchiato.com/unicode/convert.html

    static const unsigned char tamil_utf8[] =
    {
        0XE0, 0XAE, 0XAE, 0XE0, 0XAF,
        0X8A, 0XE0, 0XAE, 0XB4, 0XE0,
        0XAE, 0XBF, 0XE0, 0XAE, 0X95,
        0XE0, 0XAE, 0XB3, 0XE0, 0XAE,
        0XBF
    };

    static const wchar_t tamil_utf16[] =
    {
        0x0BAE, 0x0BCA, 0x0BB4, 0x0BBF,
        0x0B95, 0x0BB3, 0x0BBF
    };
}

BOOST_AUTO_TEST_CASE( empty_string_converts )
{
    BOOST_CHECK( std::string()  == Utf8( std::wstring() ) );
    BOOST_CHECK( std::wstring() == Utf8( std::string()  ) );
}

BOOST_AUTO_TEST_CASE( utf8_to_unicode_converts )
{
    const std::string input( reinterpret_cast< const char* >( tamil_utf8 ), COUNT_OF( tamil_utf8 ) );
    const std::wstring expected( tamil_utf16, COUNT_OF( tamil_utf16 ) );
    const std::wstring actual = Utf8( input );
    BOOST_CHECK( expected == actual );
}

BOOST_AUTO_TEST_CASE( unicode_to_utf8_converts )
{
    const std::wstring input( tamil_utf16, COUNT_OF( tamil_utf16 ) );
    const std::string expected( reinterpret_cast< const char* >( tamil_utf8 ), COUNT_OF( tamil_utf8 ) );
    const std::string actual = Utf8( input );
    BOOST_CHECK( expected == actual );
}

BOOST_AUTO_TEST_CASE( unicode_path_to_utf8_converts )
{
    const boost::filesystem::path input( std::wstring( tamil_utf16, COUNT_OF( tamil_utf16 ) ) );
    const std::string expected( reinterpret_cast< const char* >( tamil_utf8 ), COUNT_OF( tamil_utf8 ) );
    const std::string actual = Utf8( input );
    BOOST_CHECK( expected == actual );
}
