// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __clients_kernel_pch_h_
#define __clients_kernel_pch_h_

#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

#pragma warning( push, 0 )
#include <qapplication.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qgl.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qwidget.h>
#pragma warning( pop )

#include <string>
#include <vector>
#include <map>
#include <set>

#undef GetObject

#undef max
#undef min

#include "Types.h"

#endif // __clients_kernel_pch_h_
