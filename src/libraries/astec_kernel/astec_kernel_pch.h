// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __astec_kernel_pch_h_
#define __astec_kernel_pch_h_

#pragma warning( push )
#pragma warning( disable : 4290 4702 )
#include <string>
#include <vector>
#include <map>
#include <set>
#pragma warning( pop )

#include "geometry/Types.h"
typedef std::vector< geometry::Point2f > T_PointVector;
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;


#endif // __astec_kernel_pch_h_
