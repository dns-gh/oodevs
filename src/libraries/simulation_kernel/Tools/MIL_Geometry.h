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
    template< typename T >
    static T IntersectionArea( const geometry::Polygon2< T >& polygon1, const geometry::Polygon2< T >& polygon2 );
    //@}
};

#include "MIL_Geometry.inl"

#endif // __MIL_Geometry_h_
