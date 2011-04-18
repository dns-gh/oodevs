// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanPositions_h_
#define __UrbanPositions_h_

#include "clients_kernel/UrbanPositions_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Viewport_ABC;
    class GlTools_ABC;
}

namespace urban
{
    class TerrainObject_ABC;
}

namespace sword
{
    class Location;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{s
             UrbanPositions( const urban::TerrainObject_ABC& object, const sword::Location& message, const kernel::CoordinateConverter_ABC& converter, const std::string& name );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f Barycenter() const;
    virtual geometry::Rectangle2f BoundingBox() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const urban::TerrainObject_ABC& object_;
    const std::string name_;
    geometry::Polygon2f polygon_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f barycenter_;
    //@}
};

#endif // __UrbanPositions_h_
