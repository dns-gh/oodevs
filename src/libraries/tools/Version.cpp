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

#ifndef APP_MAJOR_VERSION
#   define APP_MAJOR_VERSION 4.3.3
#endif

#ifndef APP_PROJECT_VERSION
#   define APP_PROJECT_VERSION 1.0.4
#endif

#ifndef APP_VERSION
#   define APP_VERSION APP_MAJOR_VERSION##.dev
#endif

//$$$ Crap for Thales
#ifndef APP_MODEL_VERSION
#   define APP_MODEL_VERSION 4.8.3
#endif

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppVersion()
{
    return BOOST_PP_STRINGIZE( APP_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppMajorVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppMajorVersion()
{
    return BOOST_PP_STRINGIZE( APP_MAJOR_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppVersion
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
const char* tools::AppModelVersion()
{
    return BOOST_PP_STRINGIZE( APP_MODEL_VERSION );
}

// -----------------------------------------------------------------------------
// Name: tools::AppProjectVersion
// Created: JSR 2012-01-30
// -----------------------------------------------------------------------------
const char* tools::AppProjectVersion()
{
    return BOOST_PP_STRINGIZE( APP_PROJECT_VERSION );
}
