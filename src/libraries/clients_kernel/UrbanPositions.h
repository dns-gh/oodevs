// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#ifndef __kernel_UrbanPositions_h_
#define __kernel_UrbanPositions_h_

#include "UrbanPositions_ABC.h"
#include "UrbanTypes.h"

namespace kernel
{
    class UrbanObject_ABC;

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions declaration
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions : public UrbanPositions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object );
             UrbanPositions( EUrbanLevel level, const UrbanObject_ABC& object, std::vector< geometry::Point2f > positions );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual void SetInfrastructurePresent();
    virtual void SetSelection( bool selected );
    virtual bool IsSelected() const;
    virtual const geometry::Point2f& Barycenter() const;
    virtual const geometry::Rectangle2f& BoundingBox() const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual const geometry::Polygon2f& Polygon() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual bool IsInside( const geometry::Rectangle2f& rectangle ) const;
    virtual float ComputeArea() const;
    //@}

    //! @name Drawable_ABC
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeCachedValues( std::vector< geometry::Point2f >& points );
    void ComputeConvexHull( std::vector< geometry::Point2f >& points ) const;
    void EliminateRedundantVertices( const T_PointVector& vertices, float epsilon );
    void ChopSpikes( float epsilon );
    //@}

protected:
    //! @name Member data
    //@{
    EUrbanLevel            level_;
    const UrbanObject_ABC& object_;
    geometry::Polygon2f    polygon_;
    geometry::Rectangle2f  boundingBox_;
    geometry::Point2f      barycenter_;
    float                  area_;
    bool                   selected_;
    bool                   hasInfrastructure_;
    //@}

    //! @name Static Member
    //@{
    static float           epsilon_;
    //@}
};

} //! namespace kernel

#endif // __kernel_UrbanPositions_h_
