// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef HOST_TEST_H__
#define HOST_TEST_H__

#include "runtime/Io.h"

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

struct NilReader : public io::Reader_ABC
{
    int Read( void* /*dst*/, size_t /*size*/ ) { return 0; }
};

#endif // HOST_TEST_H__
