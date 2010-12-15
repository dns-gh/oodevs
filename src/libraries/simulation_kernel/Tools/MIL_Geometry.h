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

#include <geometry/Types.h>

// =============================================================================
/** @class  MIL_Geometry
    @brief  MIL Geometry
*/
// Created: SLG 2010-04-30
// =============================================================================
class MIL_Geometry : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Geometry();
    virtual ~MIL_Geometry();
    //@}

    //! @name Operations
    //@{
    static void Scale( geometry::Polygon2f& result, const geometry::Polygon2f& polygon, float distance ); // $$$$ MCO : where are the unit tests ?!
    static void ComputeHull( geometry::Polygon2f& result, const geometry::Polygon2f& polygon );
    static float IntersectionArea( const geometry::Polygon2f& polygon1, const geometry::Polygon2f& polygon2 );
    //@}
};

#endif // __MIL_Geometry_h_
