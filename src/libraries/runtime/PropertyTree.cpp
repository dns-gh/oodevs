// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PropertyTree.h"
#include "Io.h"

#ifndef BOOST_SPIRIT_THREADSAFE
#error BOOST_SPIRIT_THREADSAFE define is required
#endif

#include <boost/bind.hpp>
#include <boost/property_tree/xml_parser.hpp>

#ifdef _MSC_VER
#   pragma warning( push, 0 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

using namespace property_tree;
typedef T_Tree Tree;

namespace boost
{
namespace property_tree
{
namespace json_parser
{
    // Create necessary escape sequences from illegal characters
    template<>
    std::basic_string<char> create_escapes(const std::basic_string<char> &s)
    {
        std::basic_string<char> result;
        std::basic_string<char>::const_iterator b = s.begin();
        std::basic_string<char>::const_iterator e = s.end();
        while (b != e)
        {
            // This assumes an ASCII superset. But so does everything in PTree.
            // We escape everything outside ASCII, because this code can't
            // handle high unicode characters.
            if (*b == 0x20  || *b == 0x21  || (*b >= 0x23 && *b <= 0x2E) ||
               (*b >= 0x30  && *b <= 0x5B) || (*b >= 0x5D && *b <= 0xFF)  //it fails here because char are signed
            || (*b >= -0x80 && *b < 0)) // this will pass UTF-8 signed chars
                result += *b;
            else if (*b == char('\b')) result += char('\\'), result += char('b');
            else if (*b == char('\f')) result += char('\\'), result += char('f');
            else if (*b == char('\n')) result += char('\\'), result += char('n');
            else if (*b == char('\r')) result += char('\\'), result += char('r');
            else if (*b == char('/'))  result += char('\\'), result += char('/');
            else if (*b == char('"'))  result += char('\\'), result += char('"');
            else if (*b == char('\\')) result += char('\\'), result += char('\\');
            else
            {
                const char *hexdigits = "0123456789ABCDEF";
                typedef make_unsigned<char>::type UCh;
                unsigned long u = (std::min)(static_cast<unsigned long>(static_cast<UCh>(*b)), 0xFFFFul);
                int d1 = u / 4096; u -= d1 * 4096;
                int d2 = u / 256; u -= d2 * 256;
                int d3 = u / 16; u -= d3 * 16;
                int d4 = u;
                result += char('\\'); result += char('u');
                result += char(hexdigits[d1]); result += char(hexdigits[d2]);
                result += char(hexdigits[d3]); result += char(hexdigits[d4]);
            }
            ++b;
        }
        return result;
    }
}
}
}

namespace
{
template< typename T >
Tree Read( const std::string& text, const T& functor )
{
    Tree tree;
    std::stringstream data( text );
    if( data.peek() != std::iostream::traits_type::eof() )
        try
        {
            functor( data, tree );
        } catch( ... ) {}
    return tree;
}

template< typename T >
std::string Write( const Tree& tree, const T& functor )
{
    std::ostringstream out;
    try
    {
        functor( out, tree );
    } catch( ... ) {}
    return out.str();
}
}

// -----------------------------------------------------------------------------
// Name: ToJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
std::string property_tree::ToJson( const Tree& tree, bool isPretty )
{
    const std::string reply = Write( tree, boost::bind( &boost::property_tree::write_json< Tree >, _1, _2, isPretty ) );
    return reply.empty() ? reply : reply.substr( 0, reply.size() - 1 );
}

// -----------------------------------------------------------------------------
// Name: FromJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
Tree property_tree::FromJson( const std::string& data )
{
    return Read( data, boost::bind( &boost::property_tree::read_json< Tree >, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: FromJson
// Created: BAX 2012-08-27
// -----------------------------------------------------------------------------
Tree property_tree::FromJson( io::Reader_ABC& src )
{
    std::vector< char > buffer( 1<<12 );
    std::string data;
    int len;
    while( (len = src.Read( &buffer[0], buffer.size() ) ) > 0 )
        data.append( &buffer[0], len );
    return Read( data, boost::bind( &boost::property_tree::read_json< Tree >, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: ToXml
// Created: BAX 2012-05-21
// -----------------------------------------------------------------------------
std::string property_tree::ToXml( const Tree& tree )
{
    return Write( tree, boost::bind( &boost::property_tree::write_xml< Tree >,
                  _1, _2, boost::property_tree::xml_writer_make_settings( ' ', 4 ) ) );
}

// -----------------------------------------------------------------------------
// Name: FromXml
// Created: BAX 2012-05-21
// -----------------------------------------------------------------------------
Tree property_tree::FromXml( const std::string& data )
{
    return Read( data, boost::bind( &boost::property_tree::read_xml< Tree >, _1, _2, 0 ) );
}
