// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatPositions_h_
#define __AutomatPositions_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Moveable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  AutomatPositions
    @brief  AutomatPositions
*/
// Created: AGE 2006-10-06
// =============================================================================
class AutomatPositions : public kernel::Moveable_ABC
                       , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatPositions( const kernel::Entity_ABC& automat );
    virtual ~AutomatPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual bool CanAggregate() const;
    virtual void Move( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatPositions( const AutomatPositions& );            //!< Copy constructor
    AutomatPositions& operator=( const AutomatPositions& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& automat_;
    //@}
};

#endif // __AutomatPositions_h_
