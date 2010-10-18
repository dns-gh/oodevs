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
#include <boost/preprocessor/stringize.hpp>

#ifndef APP_VERSION
#   define APP_VERSION dev
#endif

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return BOOST_PP_STRINGIZE( APP_VERSION );
}
