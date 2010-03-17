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

#include "clients_kernel/Drawable_ABC.h"
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
                       , public kernel::Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatPositions( const kernel::Entity_ABC& automat );
    virtual ~AutomatPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual void Move( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatPositions( const AutomatPositions& );            //!< Copy constructor
    AutomatPositions& operator=( const AutomatPositions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& automat_;
    //@}
};

#endif // __AutomatPositions_h_
