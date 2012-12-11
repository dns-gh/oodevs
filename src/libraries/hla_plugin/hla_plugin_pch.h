// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_plugin_pch_h
#define plugins_hla_plugin_pch_h

// Ignore snprintf is insecure warnings.
#define _SCL_SECURE_NO_WARNINGS

#include <string>
#include <memory>
#include <map>
#include <vector>

#include <tools/Exception.h>

#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4503 ) // decorated name length exceeded, name was truncated
#pragma warning( disable : 4996 ) // 'sprintf': This function or variable may be unsafe.

#endif // plugins_hla_plugin_pch_h
