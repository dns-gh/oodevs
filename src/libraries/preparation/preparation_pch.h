// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_pch_h_
#define __preparation_pch_h_

#pragma warning( disable : 4290 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#undef Yield
#undef GetObject
#undef GetMessage

#pragma warning( push, 0 )
#include <qdatetime.h>
#include <qgl.h>
#include <qregexp.h>
#include <qstring.h>
#pragma warning( pop )

#include <string>
#include <map>
#include <vector>
#include <set>

#include <geometry/Types.h>
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#endif // __preparation_pch_h_
