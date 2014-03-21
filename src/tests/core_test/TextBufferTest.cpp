// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"

#define private public
#include "core/TextBuffer.h"

using core::TextBuffer;

namespace
{
void CheckTextBuffer( const TextBuffer& value, const char* raw, bool copy )
{
    if( copy )
    {
        BOOST_REQUIRE( !value.data_.empty() );
        BOOST_REQUIRE( value.raw_ == value.data_.c_str() );
    }
    else
    {
        BOOST_REQUIRE( value.data_.empty() );
        BOOST_REQUIRE( value.raw_ == raw );
    }
}
void CheckOperators( const char* value, bool copy )
{
    TextBuffer a( value, strlen( value ) );
    if( copy )
        a.Store();
    CheckTextBuffer( a, value, copy );
    TextBuffer b = a;
    BOOST_CHECK( b == a );
    CheckTextBuffer( b, value, copy );
    TextBuffer c( b );
    BOOST_CHECK( c == a );
    CheckTextBuffer( c, value, copy );
    TextBuffer d;
    d = c;
    CheckTextBuffer( d, value, copy );
    BOOST_CHECK( d == a );
}

const char small[] = "_";
const char large[] = "a very long text which must be bigger than default std::string buffer so we avoid short-string optimisation, which is around 20 bytes in msvc";
}

BOOST_AUTO_TEST_CASE( text_buffer_avoid_copy_unless_stored )
{
    CheckOperators( small, false );
    CheckOperators( large, false );
}

BOOST_AUTO_TEST_CASE( text_buffer_always_copy_if_stored )
{
    CheckOperators( small, true );
    CheckOperators( large, true );
}
