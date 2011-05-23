// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gaming_pch_h_
#define __gaming_pch_h_

#pragma warning( disable : 4290 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4996 )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <qstring.h>
#include <qgl.h>
#include <qdatetime.h>
#pragma warning( pop )

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

#include <geometry/Types.h>
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Extension_ABC.h"

#undef uint

#endif // __gaming_pch_h_
