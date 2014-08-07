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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Positions.h"

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
                          , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregatedPositions( const kernel::Entity_ABC& automat, float factor );
    virtual ~AggregatedPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool ) const;
    virtual float GetHeight( bool ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AggregatedPositions( const AggregatedPositions& );            //!< Copy constructor
    AggregatedPositions& operator=( const AggregatedPositions& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateChildrenPositions() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    float factor_;
    mutable T_PointVector children_;
    //@}
};

#endif // __AutomatPositions_h_
