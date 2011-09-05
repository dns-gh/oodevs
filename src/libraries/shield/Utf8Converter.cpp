// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#include "Utf8Converter.h"

using namespace shield;

using namespace google::protobuf;

// -----------------------------------------------------------------------------
// Name: Utf8Converter::AnsiToUtf8
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::AnsiToUtf8( const std::string &ansiString, std::string& utf8String )
{
    const char* ansiContent = ansiString.c_str();
    unsigned char* result = new unsigned char[ansiString.size() * 4];
    unsigned int n ( 0 );
    unsigned int  i ( ansiString.size() );
    const unsigned char* incoming = reinterpret_cast< const unsigned char* >( ansiContent );
    while ( i > 0 )
    {
        unsigned int character = static_cast< unsigned int >( *incoming );
        if ( character < 0x80 )
        {
	        result[n++] = static_cast< unsigned char >( character );
        }
        else if ( character < 0x800 )
        {
	        result[n++] = static_cast< unsigned char >( 0xC0 | ( character >> 6 ) );
	        result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        else if ( character < 0x10000 )
        {
	        result[n++] = static_cast< unsigned char >( 0xE0 | ( character >> 12 ) );
	        result[n++] = static_cast< unsigned char >( 0xC0 | ( ( character >> 6 ) & 0x3F ) );
	        result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        else if ( character < 0x200000 )
        {
	        result[n++] = static_cast< unsigned char >( 0xF0 | ( character >> 18 ) );
	        result[n++] = static_cast< unsigned char >( 0xE0 | ( ( character >> 12 ) & 0x3F ) );
	        result[n++] = static_cast< unsigned char >( 0xC0 | ( ( character >> 6 ) & 0x3F ) );
	        result[n++] = static_cast< unsigned char >( 0x80 | ( character & 0x3F ) );
        }
        ++incoming;
        --i;
    }
    result[n] = 0;
    utf8String = reinterpret_cast< char* >( result );
}

// -----------------------------------------------------------------------------
// Name: Utf8Converter::Utf8ToAnsi
// Created: RPD 2011-09-05
// -----------------------------------------------------------------------------
void Utf8Converter::Utf8ToAnsi( const std::string& utf8String, std::string &ansiString )
{
    const char* utf8content = utf8String.c_str();
    unsigned char* result = new unsigned char[utf8String.size()];
    unsigned int n ( 0 );
    unsigned int i ( utf8String.size() );
    const unsigned char* incoming = reinterpret_cast< const unsigned char* >( utf8content );

    while ( i > 0 )
    {
	    unsigned int character = *incoming;
	    if ( character >= 0xF0 )
	    {
		    if ( i >= 4 )   // four-byte encoded, 21 bits
		    {
			    character = ( ( incoming[0] & 0x07 ) << 18 ) | ( ( incoming[1] & 0x3F ) << 12 ) | ( ( incoming[2] & 0x3F ) << 6 ) | ( incoming[3] & 0x3F );
		    }
		    else
		    {
			    character = '?';    //ansi conversion impossible
		    }
		    incoming += 4;
		    i -= 4;
	    }
	    else if ( character >= 0xE0 )
	    {
		    if ( i >= 3 )   // three-byte encoded, 16 bits
		    {
			    character = ( ( incoming[0] & 0x3F ) << 12 ) | ( ( incoming[1] & 0x3F ) << 6 ) | ( incoming[2] & 0x3F );
		    }
		    else
		    {
			    character = '?';
		    }
		    incoming += 3;
		    i -= 3;
	    }
	    else if ( character >= 0xC0 )
	    {
		    if ( i >= 2 ) // two-byte encoded, 11 bits
		    {
			    character = ( ( incoming[0] & 0x3F ) << 6 ) | ( incoming[1] & 0x3F );
		    }
		    else
		    {
			    character = '?';
		    }
		    incoming += 2;
		    i -= 2;
	    }
	    else
	    {
		    ++incoming;
		    --i;
	    }
	    result[n++] = static_cast< unsigned char >( ( character > 0xFF ) ? '?' : character );
    }
    result[n] = 0;
    ansiString = reinterpret_cast< char* >( result );
}
