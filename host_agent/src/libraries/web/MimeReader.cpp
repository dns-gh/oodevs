// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "MimeReader.h"

#include "AsyncStream.h"
#include "runtime/Pool_ABC.h"
#include "StreamBuffer.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <map>

using namespace web;
using runtime::Pool_ABC;

namespace
{
template< typename T >
struct PartReader : boost::noncopyable
{
    PartReader( T part, Pool_ABC& pool )
        : async_ ( part->async_ )
        , future_( pool.Go( boost::bind( &AsyncStream::Read, &async_, part->handler_ ) ) )
    {
        // NOTHING
    }
    ~PartReader()
    {
        async_.CloseWrite();
        future_.wait();
    }
    AsyncStream& async_;
    Pool_ABC::Future future_;
};
}

struct MimeReader::Part : boost::noncopyable
{
    explicit Part( const std::string& name, const MimeReader::Handler handler )
        : name_   ( name )
        , handler_( handler )
    {
        // NOTHING
    }
    ~Part()
    {
        // NOTHING
    }
    const std::string name_;
    const MimeReader::Handler handler_;
    AsyncStream async_;
};


MimeReader::MimeReader()
{
    // NOTHING
}

MimeReader::~MimeReader()
{
    // NOTHING
}

namespace
{
void Unquote( std::string& val )
{
    if( val.empty() )
        return;
    if( !( *val.begin() == '\'' && *val.rbegin() == '\''
        || *val.begin() == '"'  && *val.rbegin() == '"' ) )
        return;
    val = val.substr( 1, val.size() - 2 );
}

typedef std::map< std::string, std::string > T_Optionals;

bool ReadMimeValue( const std::string& data, std::string& value, T_Optionals& opt )
{
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, data, boost::is_any_of( ";" ) );
    if( tokens.size() < 1 )
        return false;
    value = boost::trim_copy( tokens.front() );
    tokens.erase( tokens.begin() );
    BOOST_FOREACH( const std::string& it, tokens )
    {
        const size_t sep = it.find_first_of( '=' );
        if( sep == std::string::npos )
            continue;
        std::string subKey = it.substr( 0, sep );
        boost::trim( subKey );
        boost::to_lower( subKey );
        std::string subVal = it.substr( sep + 1 );
        boost::trim( subVal );
        Unquote( subVal );
        opt.insert( std::make_pair( subKey, subVal ) );
    }
    return true;
}

bool ReadMimeHeader( const std::string& data, std::string& key, std::string& val, T_Optionals& opt )
{
    const size_t next = data.find_first_of( ":" );
    if( next == std::string::npos )
        return false;
    key = data.substr( 0, next );
    boost::trim( key );
    boost::to_lower( key );
    return ReadMimeValue( data.substr( next+1 ), val, opt );
}

int SkipSpaces( StreamBuffer& buf )
{
    int skip = 0;
    while( !buf.Eof() )
    {
        const char next = buf.Peek();
        if( next != ' ' && next != '\t' )
            break;
        skip++;
        buf.Skip(1);
    }
    return skip;
}

std::string ReadMimeLine( StreamBuffer& buf )
{
    std::string rpy = buf.GetLine();
    if( rpy.empty() || rpy == "\r" )
        return std::string();
    while( SkipSpaces( buf ) )
        rpy += " " + buf.GetLine();
    return rpy;
}

std::string ParseHeaders( StreamBuffer& buf )
{
    std::string rpy;
    while( !buf.Eof() )
    {
        const std::string line = ReadMimeLine( buf );
        if( line.empty() ) break;

        std::string key, val; T_Optionals opt;
        bool valid = ReadMimeHeader( line, key, val, opt );
        if( !valid ) continue;
        if( key != "content-disposition" ) continue;
        if( val != "form-data" ) continue;

        T_Optionals::const_iterator it = opt.find( "name" );
        if( it == opt.end() ) continue;

        rpy = it->second;
    }
    return rpy;
}

bool IsSpaceOnly( const std::string& line )
{
    for( size_t i = 0; i < line.size(); ++i )
        if( !isspace( line[i] ) )
            return false;
    return true;
}

bool IsDelimiterLine( const std::string& line, const std::string& delimiter )
{
    if( !boost::starts_with( line, delimiter ) )
        return false;
    return IsSpaceOnly( line.substr( delimiter.size(), line.size() - 1 ) );
}

template< typename T >
bool IsName( const std::string& name, T part )
{
    return !name.empty() && name == part->name_;
}

// naive string search function (use boyer-moore instead..)
size_t FindNeedle( const char* haystack, size_t hsize, const char* needle, size_t nsize )
{
    const char first = needle[0];
    const char* ptr = haystack;
    if( hsize < nsize )
        return hsize;
    for(;;)
    {
        ptr = static_cast< const char* >( memchr( ptr, first, hsize - ( ptr - haystack ) - nsize ) );
        if( !ptr )
            break;
        if( !memcmp( ptr, needle, nsize ) )
            return ptr - haystack;
        ++ptr;
    }
    return hsize;
}

template< typename T >
void ParseData( StreamBuffer& buf, const std::string boundary, T part = T() )
{
    const std::string end = "\r\n--" + boundary;
    while( !buf.Eof() )
    {
        char* ptr;
        const size_t read = buf.Peek( &ptr );
        const size_t next = FindNeedle( ptr, read, end.c_str(), end.size() );
        if( next != read )
        {
            // consume all until delimiter
            if( part )
                part->async_.Write( ptr, next );
            buf.Skip( next + 2 );
            return;
        }
        // eat last chunk if not big enough to contain a delimiter
        const size_t skip = read <= end.size() ? read : read - end.size();
        // consume all except potentially truncated boundary
        if( part )
            part->async_.Write( ptr, skip );
        buf.Skip( skip );
    }
}

template< typename T >
void ParsePart( StreamBuffer& buf, const std::string boundary, T part, Pool_ABC& pool )
{
    PartReader< T > reader( part, pool );
    ParseData( buf, boundary, part );
}

template< typename T >
typename T::value_type NextPart( T& data, const std::string& name )
{
    typedef typename T::value_type U;
    typename T::iterator it = std::find_if( data.begin(), data.end(), boost::bind( &IsName< U >, name, _1 ) );
    if( it == data.end() )
        return U();
    U part = *it;
    data.erase( data.begin(), it + 1 );
    return part;
}
}

void MimeReader::PutHeader( const std::string& name, const std::string& value )
{
    std::vector< char > data;
    const std::string key = boost::to_lower_copy( name );
    std::string val; T_Optionals opt;
    if( key != "content-type" )
        return;
    if( !ReadMimeValue( value, val, opt ) )
        return;
    if( !boost::iequals( val, "multipart/form-data" ) )
        return;
    T_Optionals::const_iterator it = opt.find( "boundary" );
    if( it == opt.end() )
        return;
    boundary_ = it->second;
}

bool MimeReader::IsValid() const
{
    return !boundary_.empty();
}

void MimeReader::Register( const std::string& name, const Handler& handler )
{
    parts_.push_back( boost::make_shared< Part >( name, handler ) );
}

void MimeReader::Parse( Pool_ABC& pool, std::istream& src )
{
    StreamBuffer buf( src );
    const std::string end = "--" + boundary_;
    while( !buf.Eof() )
    {
        const std::string line = ReadMimeLine( buf );
        if( IsDelimiterLine( line, end + "--" ) )
            break;
        if( !IsDelimiterLine( line, end ) )
            break;
        T_Part part = NextPart( parts_, ParseHeaders( buf ) );
        if( part )
            ParsePart( buf, boundary_, part, pool );
        else
            ParseData< T_Part >( buf, boundary_ );
    }
}
