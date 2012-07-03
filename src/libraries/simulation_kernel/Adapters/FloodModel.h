// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_FLOOD_MODEL_H
#define SWORD_FLOOD_MODEL_H

#include "flood/FloodModel_ABC.h"

namespace sword
{
// =============================================================================
/** @class  FloodModel
    @brief  Flood model
*/
// Created: LGY 2012-06-13
// =============================================================================
class FloodModel : public flood::FloodModel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodModel();
    virtual ~FloodModel();
    //@}

    //! @name Operations
    //@{
    virtual bool GenerateFlood( const geometry::Point2d& center, int depth, int refDist, bool force = false );
    virtual const std::vector< geometry::Polygon2f* >& GetDeepAreas() const;
    virtual const std::vector< geometry::Polygon2f* >& GetLowAreas() const;
    //@}
};

}

#endif // SWORD_FLOOD_MODEL_H
