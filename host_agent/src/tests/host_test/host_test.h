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

std::string BOOST_RESOLVE( const std::string& filename );

struct NilReader : public io::Reader_ABC
{
    size_t Read( void* /*dst*/, size_t /*size*/ ) { return 0; }
};

#endif // HOST_TEST_H__