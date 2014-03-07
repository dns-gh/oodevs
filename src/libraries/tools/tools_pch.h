// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __tools_pch_h_
#define __tools_pch_h_

#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4503 ) // decorated name length exceeded, name was truncated

#include <string>
#include "asio.h"

#undef max
#undef min

#include <tools/Exception.h>
#include <tools/Helpers.h>

#include <geometry/Types.h>

#endif // __tools_pch_h_
