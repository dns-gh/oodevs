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

#include "Drawable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Positions.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  AggregatedPositions
    @brief  AggregatedPositions
*/
// Created: AGE 2006-10-06
// =============================================================================
class AggregatedPositions : public kernel::Positions
                          , public gui::Drawable_ABC
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregatedPositions( const kernel::Entity_ABC& automat );
    virtual ~AggregatedPositions();
    //@}

    //! @name Positions
    //@{
    virtual geometry::Point2f GetPosition( bool ) const;
    virtual float GetHeight( bool ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

    //! @name Drawable_ABC
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

    //! @name Moveable_ABC
    //@{
    virtual void Move( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

} //! namespace gui

#endif // __AutomatPositions_h_

