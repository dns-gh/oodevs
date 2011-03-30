// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Geometry_h_
#define __MIL_Geometry_h_

#include "MT_Tools/Mt_Vector2DTypes.h"
#include <geometry/Types.h>

class TER_Localisation;

// =============================================================================
/** @class  MIL_Geometry
    @brief  MIL Geometry
*/
// Created: SLG 2010-04-30
// =============================================================================
class MIL_Geometry
{
public:
    //! @name Operations
    //@{
    static void Scale( geometry::Polygon2f& result, const geometry::Polygon2f& polygon, float distance ); // $$$$ MCO : where are the unit tests ?!
    static void ComputeHull( geometry::Polygon2f& result, const geometry::Polygon2f& polygon );
    static void ComputeHull( T_PointVector& result, const T_PointVector& polygon );
    static float IntersectionArea( const geometry::Polygon2f& polygon1, const geometry::Polygon2f& polygon2 );
    static double IntersectionArea( const TER_Localisation& localisation1, const TER_Localisation& localisation2 );
    //@}
};

#endif // __MIL_Geometry_h_
