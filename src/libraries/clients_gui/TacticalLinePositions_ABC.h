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

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
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
                                , public Drawable_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             TacticalLinePositions_ABC( const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::TacticalLine_ABC& owner );
             TacticalLinePositions_ABC( const T_PointVector& pointList,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::TacticalLine_ABC& owner );
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
    virtual void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLinePositions_ABC( const TacticalLinePositions_ABC& );            //!< Copy constructor
    TacticalLinePositions_ABC& operator=( const TacticalLinePositions_ABC& ); //!< Assignment operator
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
    //@}

private:
    //! @name Member data
    //@{
    const kernel::TacticalLine_ABC& owner_;
    geometry::Rectangle2f boundingBox_;
    //@}
};

}

#endif // CLIENTS_GUI_TACTICALLINEPOSITIONS_ABC_H__