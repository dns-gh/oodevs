// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Base64Converters.h"
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>

using namespace boost::archive::iterators;

namespace
{
    template< typename InputContainerType >
    inline std::string Base64Encode( InputContainerType cont )
    {
        using namespace boost::archive::iterators;
        typedef insert_linebreaks< base64_from_binary< transform_width< std::vector< char >::const_pointer, 6, 8 > >, 72 > T_Base64;
        unsigned int padding = ( 3 - cont.size() % 3 ) % 3;
        for( unsigned int i = 0; i < padding; ++i )
            cont.push_back( 0x00 );
        std::string result = std::string( T_Base64( &cont[ 0 ] ), T_Base64( &cont[ 0 ] + cont.size() - padding ) );
        result.append( padding, '=' );
        return result;
    }

    template< typename Container, typename CharT >
    inline Container Base64Decode( const std::basic_string< CharT >& str )
    {
        typedef transform_width< binary_from_base64< remove_whitespace< CharT * > >, 8, 6 > T_Binary;
        typename std::basic_string< CharT >::size_type pos = str.find_last_not_of( CharT( '=' ) );
        pos = ( pos == str.size() - 1 ? str.size() : pos );
        return Container( T_Binary( &str[0] ), T_Binary( &str[0] + pos ) );
    }
}

// -----------------------------------------------------------------------------
// Name: tools::BinaryToBase64
// Created: ABR 2013-05-27
// -----------------------------------------------------------------------------
std::string tools::BinaryToBase64( const std::string& binary )
{
    return Base64Encode( binary );
}

// -----------------------------------------------------------------------------
// Name: tools::Base64ToBinary
// Created: ABR 2013-05-27
// -----------------------------------------------------------------------------
std::string tools::Base64ToBinary( const std::string& base64 )
{
    return Base64Decode< std::string >( base64 );
}
