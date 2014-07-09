// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef TER_GEOMETRY_H
#define TER_GEOMETRY_H

#include "MT_Tools/Mt_Vector2DTypes.h"

class TER_Localisation;
class TER_Polygon;

class TER_Geometry
{
public:
    //@{
    static void Scale( TER_Polygon& result, const T_PointVector& polygon, double distance ); // $$$$ MCO : where are the unit tests ?!
    static void ComputeHull( T_PointVector& result, const T_PointVector& polygon );
    static double IntersectionArea( const TER_Localisation& localisation1, const TER_Localisation& localisation2 );
    static bool IsEntirelyCovered( const TER_Localisation& toCover, const std::vector< TER_Localisation >& covers );
    //@}
};

#endif // TER_GEOMETRY_H
