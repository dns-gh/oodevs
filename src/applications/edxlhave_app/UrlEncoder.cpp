// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "edxlhave_app_pch.h"
#include "UrlEncoder.h"

using namespace edxl;

// -----------------------------------------------------------------------------
// Name: UrlEncoder constructor
// Created: JCR 2010-07-23
// -----------------------------------------------------------------------------
UrlEncoder::UrlEncoder( const std::string& message )
    : message_ ( message )
{
    // strip message
    // remove leading whitespace in XML to reduce message size
    std::string::size_type pos = 0;
    while ( ( pos = message_.find("\n  ", pos ) ) != std::string::npos )
        message_.erase( pos + 1, 2);
    Encode( message_ );
}

// -----------------------------------------------------------------------------
// Name: UrlEncoder destructor
// Created: JCR 2010-07-23
// -----------------------------------------------------------------------------
UrlEncoder::~UrlEncoder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrlEncoder::Write
// Created: JCR 2010-07-23
// -----------------------------------------------------------------------------
void UrlEncoder::Write( std::ostream& os ) const
{
    os << "Content-length: " << message_.length() <<"\r\n";
    os << "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
    os << "message=update&data=" << message_ << "\r\n\r\n";
}

// -----------------------------------------------------------------------------
// Name: UrlEncoder::Encode
// Created: JCR 2010-07-23
// -----------------------------------------------------------------------------
void UrlEncoder::Encode( const std::string& message )
{
    std::string res;
    for( unsigned i = 0; i < message.length(); i++ )
        switch( message[i] )
        {
            case '%':
                res += "%25";
                break;
            case ' ':
                res += "%20";
                break;
            case '^':
                res += "%5E";
                break;
            case '&':
                res += "%26";
                break;
            case '`':
                res += "%60";
                break;
            case '{':
                res+="%7B";
                break;
            case '}':
                res+="%7D";
                break;
            case '|':
                res+="%7C";
                break;
            case ']':
                res+="%5D";
                break;
            case '[':
                res+="%5B";
                break;
            case '"':
                res+="%22";
                break;
            case '<':
                res+="%3C";
                break;
            case '>':
                res+="%3E";
                break;
            case '\\':
                res+="%5C";
                break;
            case '#':
                res+="%23";
                break;
            case '?':
                res+="%3F";
                break;
            case '/':
                res+="%2F";
                break;
            default:
                res += message[i];
                break;
        }
    message_.swap( res );
}