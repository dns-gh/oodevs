// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_TACTICALLINEPOSITIONS_ABC_H__
#define CLIENTS_GUI_TACTICALLINEPOSITIONS_ABC_H__

#include "Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/LocationVisitor_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
    class TacticalLine_ABC;
    class Controller;
}

namespace gui
{
// =============================================================================
/** @class  TacticalLinePositions
    @brief  TacticalLine Positions
*/
// Created: MMC 2012-05-11
// =============================================================================
class TacticalLinePositions_ABC : public kernel::Positions
                                , public kernel::LocationVisitor_ABC
                                , public Drawable_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions_ABC( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::TacticalLine_ABC& owner );
             TacticalLinePositions_ABC( kernel::Controller& controller, const T_PointVector& pointList,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::TacticalLine_ABC& owner );
    virtual ~TacticalLinePositions_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float             GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const Viewport_ABC& viewport, GlTools_ABC& tools ) const;
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
    virtual void VisitCurve( const T_PointVector& points );
    virtual void VisitText( const QString&, const QFont&, const geometry::Point2f& ) {}
    //@}

protected:
    //! @name Helpers
    //@{
    void ComputeBoundingBox();
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    T_PointVector pointList_;
    std::unique_ptr< kernel::Location_ABC > location_;
    const kernel::TacticalLine_ABC& owner_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

}

#endif // CLIENTS_GUI_TACTICALLINEPOSITIONS_ABC_H__