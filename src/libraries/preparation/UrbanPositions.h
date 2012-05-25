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

#include "UrbanTypes.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class UrbanObject_ABC;
}

// =============================================================================
/** @class  UrbanPositions
    @brief  Urban positions
*/
// Created: JSR 2010-09-06
// =============================================================================
class UrbanPositions : public kernel::UrbanPositions_ABC
                     , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanPositions( xml::xistream& xis, EUrbanLevel level, const kernel::UrbanObject_ABC& object, const kernel::CoordinateConverter_ABC& converter );
    virtual ~UrbanPositions();
    //@}

    //! @name Operations
    //@{
    virtual const geometry::Point2f& Barycenter() const;
    virtual const geometry::Rectangle2f& BoundingBox() const;
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual bool IsInside( const geometry::Rectangle2f& rectangle ) const;
    virtual const std::vector< geometry::Point2f >& Vertices() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual float ComputeArea() const;
    virtual void SetSelection( bool selected );
    virtual void SetInfrastructurePresent();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis, std::vector< geometry::Point2f >& positions ) const;
    void ComputeConvexHull( std::vector< geometry::Point2f >& points ) const;
    void ComputeCachedValues( std::vector< geometry::Point2f >& points );
    void EliminateRedundantVertices( const T_PointVector& vertices, float epsilon );
    void ChopSpikes( float epsilon );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    EUrbanLevel level_;
    const kernel::UrbanObject_ABC& object_;
    bool selected_;
    bool hasInfrastructure_;
    geometry::Polygon2f polygon_;
    geometry::Point2f barycenter_;
    geometry::Rectangle2f boundingBox_;
    float area_;
    //@}

    //! @name Static Member
    //@{
    static float epsilon_;
    //@}
};

#endif // __UrbanPositions_h_
