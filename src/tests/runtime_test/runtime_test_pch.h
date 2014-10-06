// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef runtime_test_pch_h__
#define runtime_test_pch_h__

#ifdef _MSC_VER
#pragma warning( disable: 4505 )
#endif

#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include <tools/BoostTest.h>

extern tools::TestOptions testOptions;

#endif // runtime_test_pch_h__
