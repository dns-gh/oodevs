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
    // Computes the convex hull of the input polygon then extents it with a buffer of
    // "distance" radius. "polygon" can contains a single point or a line. Negative
    // distances are forced to zero.
    static void Buffer( TER_Polygon& result, const T_PointVector& polygon, double distance );
    // Returns the convex hull of the input point set. If there are less than
    // two points or if they are colinear, returns the input point set.
    static void ComputeHull( T_PointVector& result, const T_PointVector& points );
    static double IntersectionArea( const TER_Localisation& localisation1, const TER_Localisation& localisation2 );
    static bool IsEntirelyCovered( const TER_Localisation& toCover, const std::vector< TER_Localisation >& covers );
};

#endif // TER_GEOMETRY_H
