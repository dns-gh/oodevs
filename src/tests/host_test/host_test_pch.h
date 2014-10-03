// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef HOST_TEST_PCH_H__
#define HOST_TEST_PCH_H__

#include "runtime/Io.h"

#ifdef _MSC_VER
#pragma warning( disable: 4505 4724 )
#endif

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

struct NilReader : public io::Reader_ABC
{
    int Read( void* /*dst*/, size_t /*size*/ ) { return 0; }
};

#endif // HOST_TEST_PCH_H__
