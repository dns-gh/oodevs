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
#include <qapplication.h>

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

namespace
{
    struct Translation : public QObject
    {
        Translation()
        {
            qApp->translate( "Application", "SWORD" );
            qApp->translate( "Application", "sword-ot@masagroup.net" );
            qApp->translate( "Application", "© 2008 MASA Group" );
            qApp->translate( "Application", "www.masagroup.net" );
            qApp->translate( "Application", "images/gui/splash_swordot.jpg" );
        }
    };
}