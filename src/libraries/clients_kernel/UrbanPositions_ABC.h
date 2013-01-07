// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#ifndef __kernel_UrbanPositions_ABC_h_
#define __kernel_UrbanPositions_ABC_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"
#include "Drawable_ABC.h"
#include "UrbanSelection_ABC.h"

namespace kernel
{
// =============================================================================
/** @class  UrbanPositions_ABC
    @brief  Urban positions declaration
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions_ABC : public Extension_ABC
                         , public Displayable_ABC
                         , public Drawable_ABC
                         , public UrbanSelection_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions_ABC() {}
    virtual ~UrbanPositions_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const geometry::Point2f& Barycenter() const = 0;
    virtual const geometry::Rectangle2f& BoundingBox() const = 0;
    virtual float ComputeArea() const = 0;
    virtual const std::vector< geometry::Point2f >& Vertices() const = 0;
    virtual const geometry::Polygon2f& Polygon() const = 0;
    virtual void ChangeGeometry( std::vector< geometry::Point2f >& points ) = 0;
    virtual bool IsInside( const geometry::Point2f& point ) const = 0;
    virtual bool IsInside( const geometry::Rectangle2f& rectangle ) const = 0;
    virtual void SetInfrastructurePresent( bool present ) = 0;
    virtual void ResetConvexHull() = 0;
    //@}
};

} //! namespace kernel

#endif // __kernel_UrbanPositions_ABC_h_
