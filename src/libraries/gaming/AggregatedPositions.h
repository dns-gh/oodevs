// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AggregatedPositions_h_
#define __AggregatedPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  AggregatedPositions
    @brief  AggregatedPositions
*/
// Created: AGE 2006-10-06
// =============================================================================
class AggregatedPositions : public kernel::Positions
                          , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AggregatedPositions( const kernel::Entity_ABC& automat );
    virtual ~AggregatedPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool ) const;
    virtual float GetHeight( bool ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AggregatedPositions( const AggregatedPositions& );            //!< Copy constructor
    AggregatedPositions& operator=( const AggregatedPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateChildrenPositions() const;
    void ComputeHull() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    mutable T_PointVector children_;
    //@}
};

#endif // __AutomatPositions_h_
