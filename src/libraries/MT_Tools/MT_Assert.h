// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_Assert_h_
#define __MT_Assert_h_

#include <cassert>
#include <iostream>
#include <intrin.h>

//-----------------------------------------------------------------------------
// Name: MT_Assert
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
inline
bool MT_Assert( const char* expression, const char* file, int line )
{
    std::cerr << "assert '" << expression
              << "' - File '" << file
              << "' - Line "  << line << std::endl;
    __debugbreak();
    return true;
}

#undef assert

#ifdef  NDEBUG
#   define assert( exp ) ( static_cast< void >( exp ) )
#else
#   define assert( exp ) ( (exp) || MT_Assert( #exp, __FILE__, __LINE__ ) )
#endif  // NDEBUG

#endif // __MT_Assert_h_
