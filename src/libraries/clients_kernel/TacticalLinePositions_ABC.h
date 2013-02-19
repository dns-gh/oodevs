// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_TacticalLinePositions_ABC_h_
#define __kernel_TacticalLinePositions_ABC_h_

#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Positions.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class TacticalLine_ABC;

// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: MMC 2012-05-11
// =============================================================================
class TacticalLinePositions_ABC : public kernel::Positions
                                , public kernel::LocationVisitor_ABC
                                , public kernel::Drawable_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions_ABC( const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
             TacticalLinePositions_ABC( const T_PointVector& pointList, const kernel::CoordinateConverter_ABC& converter, const TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

protected:
    //! @name LocationVisitor_ABC
    //@{
    virtual void VisitLines( const T_PointVector& points );
    virtual void VisitPoint( const geometry::Point2f& point );
    virtual void VisitRectangle( const T_PointVector& ) {}
    virtual void VisitPolygon( const T_PointVector& ) {}
    virtual void VisitCircle( const geometry::Point2f&, float ) {}
    virtual void VisitPath( const geometry::Point2f&, const T_PointVector& ) {}
    //@}

protected:
    //! @name Helpers
    //@{
    void ComputeBoundingBox();
    //@}

    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector pointList_;
    std::auto_ptr< kernel::Location_ABC > location_;
    const kernel::TacticalLine_ABC& owner_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

}

#endif // __kernel_TacticalLinePositions_ABC_h_