// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Utf8.h"
#include <vector>
#define  BOOST_BIND_ENABLE_STDCALL
#include <boost/bind.hpp>
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
        int required = converter( text.data(), text.size(), reinterpret_cast< V* >( 0 ), 0 );
        if( required <= 0 )
            throw std::runtime_error( "unable to convert text" );

        std::vector< V > data( required + 1, 0 );
        int size = converter( text.data(), text.size(), &data[0], required );
        return size > 0 ? U( &data[0], required ) : U();
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
