// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Version.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <vector>
#include <windows.h>
#pragma comment(lib, "version.lib")

namespace
{
    const std::string devVersion = "5.2.0.0";

    std::wstring GetModuleFilename()
    {
        DWORD ret;
        std::vector< wchar_t > buffer( 256 );
        for(;;)
        {
            ret = ::GetModuleFileNameW( 0, &buffer[0], static_cast< DWORD >( buffer.size() ) );
            if( ret < static_cast< DWORD >( buffer.size() ) || ::GetLastError() != ERROR_INSUFFICIENT_BUFFER )
                break;
            buffer.resize( buffer.size() * 2 );
        }
        return std::wstring( &buffer[0], ret );
    }

    std::string GetAppVersion()
    {
        const std::wstring module = GetModuleFilename();
        DWORD dummy;
        const DWORD size = GetFileVersionInfoSizeW( module.c_str(), &dummy );
        if( !size )
            return devVersion;
        std::vector< uint8_t > data( size );
        DWORD err = GetFileVersionInfoW( module.c_str(), NULL, size, &data[0] );
        if( !err )
            return devVersion;
        void* buffer; UINT bufsize;
        BOOL ret = VerQueryValueW( &data[0], L"\\", &buffer, &bufsize );
        if( !ret )
            return devVersion;
        VS_FIXEDFILEINFO info;
        if( bufsize != sizeof info )
            return devVersion;
        info = *reinterpret_cast< VS_FIXEDFILEINFO* >( buffer );
        std::stringstream stream;
        stream << ( info.dwProductVersionMS >> 16    ) << "."
               << ( info.dwProductVersionMS & 0xFFFF ) << "."
               << ( info.dwProductVersionLS >> 16    ) << "."
               << ( info.dwProductVersionLS & 0xFFFF );
        return stream.str();
    }

    typedef std::vector< std::string > T_Tokens;

    T_Tokens Split( const std::string& value )
    {
        T_Tokens tokens;
        boost::algorithm::split( tokens, value, boost::is_any_of( "." ) );
        return tokens;
    }

    std::string Splice( const T_Tokens& src, size_t end )
    {
        const T_Tokens slice( src.begin(), src.begin() + std::min( end, src.size() ) );
        return boost::algorithm::join( slice, "." );
    }

    const std::string version = GetAppVersion();
    const std::string model   = "4.8.2";
    const T_Tokens    tokens  = Split( version );
    const std::string project = Splice( tokens, 2 );
    const std::string major   = Splice( tokens, 3 );
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return ::version.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppMajorVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppMajorVersion()
{
    return ::major.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppModelVersion()
{
    return ::model.c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::AppProjectVersion
// Created: JSR 2012-01-30
// -----------------------------------------------------------------------------
const char* tools::AppProjectVersion()
{
    return ::project.c_str();
}

// -----------------------------------------------------------------------------
// Name: Version::SplitVersion
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
const std::vector< int > tools::SplitVersion( const std::string& version )
{
    std::vector< int > result;
    std::transform(
        boost::sregex_token_iterator( version.begin(), version.end(), boost::regex( "(\\d+)" ) ),
        boost::sregex_token_iterator(),
        std::back_inserter( result ),
        &boost::lexical_cast< int, std::string > );
    return result;
}

// -----------------------------------------------------------------------------
// Name: Version::CheckVersion
// Created: LGY 2012-05-04
// -----------------------------------------------------------------------------
bool tools::CheckVersion( const std::string& expectedVersion, const std::string& actualVersion )
{
    const std::vector< int > expected = SplitVersion( expectedVersion );
    const std::vector< int > actual = SplitVersion( actualVersion );
    return expected.empty() || actual.empty() ||
        !std::lexicographical_compare( actual.begin(), actual.end(),
                                       expected.begin(), expected.end() );
}
