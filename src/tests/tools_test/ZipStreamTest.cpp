// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include <tools/Path.h>
#include "tools/zipextractor.h"
#include <tools/TemporaryDirectory.h>
#pragma warning( disable: 4127 4244 4245 4996 )
#include <zipstream/izipfile.h>

BOOST_AUTO_TEST_CASE( zipstream_extract_archive )
{
    tools::TemporaryDirectory dir( "zipstream-", temp_directory );
    auto path = BOOST_RESOLVE( "emptyfile.zip" );
    auto dest = tools::Path::FromUnicode( dir.path().wstring() );
    tools::zipextractor::ExtractArchive( path, dest );

    auto nonempty = dest / tools::Path::FromUTF8( "zipdir/nonempty" );
    BOOST_CHECK( nonempty.Exists() );
    auto empty = dest / tools::Path::FromUTF8( "zipdir/empty" );
    BOOST_CHECK( empty.Exists() );
}
