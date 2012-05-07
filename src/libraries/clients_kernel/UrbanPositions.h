// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#ifndef __kernel_UrbanPositions_h_
#define __kernel_UrbanPositions_h_

#include "UrbanPositions_ABC.h"

namespace kernel
{
    class UrbanColor_ABC;

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions : public UrbanPositions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( std::vector< geometry::Point2f > positions,
                             const std::string& name, const UrbanColor_ABC& color );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual const geometry::Point2f& Barycenter() const;
    virtual const geometry::Rectangle2f& BoundingBox() const;
    virtual float ComputeArea() const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual void SetInfrastructurePresent();
    virtual void ToggleSelection();
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member Data
    //@{
    const std::string name_;
    const kernel::UrbanColor_ABC& color_;
    bool selected_;
    bool hasInfrastructure_;
    float area_;
    geometry::Polygon2f polygon_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f barycenter_;
    //@}
};

}

#endif // __kernel_UrbanPositions_h_
