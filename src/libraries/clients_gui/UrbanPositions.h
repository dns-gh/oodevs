// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************_

#ifndef CLIENTS_GUI_URBANPOSITION_H__
#define CLIENTS_GUI_URBANPOSITION_H__

#include "Drawable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/UrbanTypes.h"
#include "clients_kernel/Types.h"

namespace kernel
{
    class UrbanObject_ABC;
}

namespace gui
{
    class Tesselator;
// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: LGY 2011-04-15
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions_ABC
                     , public kernel::Displayable_ABC
                     , public Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( EUrbanLevel level,
                             const kernel::UrbanObject_ABC& object,
                             Tesselator& tesselator );
             UrbanPositions( EUrbanLevel level,
                             const kernel::UrbanObject_ABC& object, std::vector< geometry::Point2f > positions,
                             Tesselator& tesselator );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual const geometry::Point2f& Barycenter() const;
    virtual const geometry::Rectangle2f& BoundingBox() const;
    virtual float ComputeArea() const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual const geometry::Polygon2f& Polygon() const;
    virtual void ChangeGeometry( std::vector< geometry::Point2f >& points );
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual bool IsInside( const geometry::Rectangle2f& rectangle ) const;
    virtual void SetInfrastructurePresent( bool present );
    virtual void SetSelection( bool selected );
    virtual bool IsSelected() const;
    virtual void ResetConvexHull();
    //@}

    //! @name Drawable_ABC
    //@{
    virtual void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GLView_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ComputeCachedValues( std::vector< geometry::Point2f >& points );
    void EliminateRedundantVertices( const T_PointVector& vertices, float epsilon );
    void ChopSpikes( float epsilon );
    //@}

protected:
    //! @name Member Data
    //@{
    EUrbanLevel level_;
    const kernel::UrbanObject_ABC& object_;
    Tesselator& tesselator_;
    bool selected_;
    bool hasInfrastructure_;
    float area_;
    geometry::Polygon2f polygon_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f barycenter_;
    kernel::T_TessellatedPolygon tessellatedPolygon_;
    //@}
};

}

#endif // CLIENTS_GUI_URBANPOSITION_H__
