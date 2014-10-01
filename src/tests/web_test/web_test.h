// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef web_test_h__
#define web_test_h__

#include "runtime/Io.h"

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

struct StreamReader : public io::Reader_ABC
{
    StreamReader( std::istream& src ) : src_( src ) {}
    int Read( void* dst, size_t size )
    {
        src_.read( reinterpret_cast< char* >( dst ), size );
        return static_cast< int >( src_.gcount() );
    }
    std::istream& src_;
};

#endif
