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

namespace flood
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

    //! @name Operations
    //@{
    virtual bool GenerateFlood( const geometry::Point2d& center, int depth, int refDist, bool force = false ) = 0;
    virtual const std::vector< geometry::Polygon2f* >& GetDeepAreas() const = 0;
    virtual const std::vector< geometry::Polygon2f* >& GetLowAreas() const = 0;
    //@}
};

}

#endif // FLOOD_FLOOD_MODEL_ABC_H
