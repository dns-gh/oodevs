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
    class UrbanColor_ABC;
}

namespace sword
{
    class Location;
    class UrbanAttributes;
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
             UrbanPositions( const sword::Location& location, const sword::UrbanAttributes& attributes,
                             const kernel::CoordinateConverter_ABC& converter, const std::string& name, const kernel::UrbanColor_ABC& color );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f Barycenter() const;
    virtual geometry::Rectangle2f BoundingBox() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual float ComputeArea() const;
    virtual void ToggleSelection();
    virtual void SetInfrastructurePresent();
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const kernel::UrbanColor_ABC& color_;
    bool selected_;
    unsigned int height_;
    bool hasInfrastructure_;
    geometry::Polygon2f polygon_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f barycenter_;
    //@}
};

#endif // __UrbanPositions_h_
