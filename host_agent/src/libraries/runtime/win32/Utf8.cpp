// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
#include "../Utf8.h"
#include <vector>
#ifndef  _WIN64
#define  BOOST_BIND_ENABLE_STDCALL
#endif
#include <boost/bind.hpp>
#define  NOMINMAX
#include <windows.h>

using namespace process;

namespace
{
    // -----------------------------------------------------------------------------
    // Name: Convert
    // Created: BAX 2012-03-07
    // -----------------------------------------------------------------------------
    template< typename T, typename U, typename V, typename W >
    U Convert( const T& text, const W& converter )
    {
        if( text.empty() )
            return U();

        const int text_size = static_cast< int >( text.size() );
        const int required = converter( text.data(), text_size, reinterpret_cast< V* >( 0 ), 0 );
        if( required <= 0 )
            throw std::runtime_error( "unable to convert text" );

        std::vector< V > data( required + 1, 0 );
        const int size = converter( text.data(), text_size, &data[0], required );
        return U( &data[0], size );
    }
}

// -----------------------------------------------------------------------------
// Name: Utf8Convert
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::wstring process::Utf8Convert( const std::string& text )
{
    return Convert< std::string, std::wstring, wchar_t >( text,
        boost::bind( MultiByteToWideChar, CP_UTF8, 0, _1, _2, _3, _4 ) );
}

// -----------------------------------------------------------------------------
// Name: Utf8Convert
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string process::Utf8Convert( const std::wstring& text )
{
    return Convert< std::wstring, std::string, char >( text,
        boost::bind( WideCharToMultiByte, CP_UTF8, 0, _1, _2, _3, _4,
            reinterpret_cast< LPCSTR >( 0 ), reinterpret_cast< LPBOOL >( 0 ) ) );
}
