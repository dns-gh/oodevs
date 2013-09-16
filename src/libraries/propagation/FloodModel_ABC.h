// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef FLOOD_FLOOD_MODEL_ABC_H
#define FLOOD_FLOOD_MODEL_ABC_H

#include "geometry/Types.h"
#include <vector>
#include <boost/noncopyable.hpp>

namespace propagation
{
// =============================================================================
/** @class  FloodModel_ABC
    @brief  Flood model definition
*/
// Created: LGY 2012-06-13
// =============================================================================
class FloodModel_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodModel_ABC() {}
    virtual ~FloodModel_ABC() {}
    //@}

   //! @name Types
   //@{
   typedef std::vector< geometry::Polygon2f > T_Polygons;
   //@}

    //! @name Operations
    //@{
    virtual void GenerateFlood( const geometry::Point2d& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const = 0;
    virtual void GenerateFlood( const geometry::Point2f& center, T_Polygons& deepAreas, T_Polygons& lowAreas, int depth, int refDist ) const = 0;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}
};

}

#endif // FLOOD_FLOOD_MODEL_ABC_H
