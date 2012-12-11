// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __tools_pch_h_
#define __tools_pch_h_

#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list

#include <string>
#include "asio.h"

#undef max
#undef min

#include <tools/Exception.h>
#include <tools/EncodingConverter.h>

#include <geometry/Types.h>

#endif // __tools_pch_h_
