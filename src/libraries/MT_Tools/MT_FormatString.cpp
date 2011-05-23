// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MT_FormatString.h"
#include <vector>
#pragma warning( disable: 4996 )
#include <cstdio>

//-----------------------------------------------------------------------------
// Name: MT_FormatString constructor
// Created: CBX 2001-06-18
//-----------------------------------------------------------------------------
MT_FormatString::MT_FormatString( const char* pFormat, ... )
{
    va_list argList;
    va_start( argList, pFormat );
    VFormat( pFormat, argList );
    va_end( argList );
}

//-----------------------------------------------------------------------------
// Name: MT_FormatString::Format
// Created: CBX 2001-06-18
//-----------------------------------------------------------------------------
void MT_FormatString::Format( const char* pFormat, ... )
{
    va_list argList;
    va_start( argList, pFormat );
    VFormat( pFormat, argList );
    va_end( argList );
}

//-----------------------------------------------------------------------------
// Name: MT_FormatString::VFormat
// Created: CBX 2001-06-18
//-----------------------------------------------------------------------------
void MT_FormatString::VFormat( const char* pFormat, va_list argList )
{
    fstr_.erase();
    if( ! pFormat )
        return;
    std::size_t nLength = 1 + 8 * strlen( pFormat );
    for( int nCount = -1; nCount < 0; nLength *= 2 )
    {
        std::vector< char > buffer( nLength );
        nCount = _vsnprintf( &buffer[0], nLength - 1, pFormat, argList );
        if( nCount > 0 )
            fstr_.assign( &buffer[0], nCount );
    }
}

//-----------------------------------------------------------------------------
// Name: MT_FormatString::operator std::string&
// Created: CBX 2002-09-03
//-----------------------------------------------------------------------------
MT_FormatString::operator const std::string&() const
{
    return fstr_;
}

//-----------------------------------------------------------------------------
// Name: MT_FormatString::c_str()
// Created: MCO 2002-11-21
//-----------------------------------------------------------------------------
const char* MT_FormatString::c_str() const
{
    return fstr_.c_str();
}
