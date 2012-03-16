// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef UTF8_H__
#define UTF8_H__
#include <string>

namespace runtime
{
// -----------------------------------------------------------------------------
// Name: Utf8Convert
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::wstring Utf8Convert( const std::string& text );

// -----------------------------------------------------------------------------
// Name: Utf8Convert
// Created: BAX 2012-03-07
// -----------------------------------------------------------------------------
std::string  Utf8Convert( const std::wstring& text );

}

#endif
