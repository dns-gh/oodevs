// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel/Moveable_ABC.h"

#ifndef __MoveableProxy_h_
#define __MoveableProxy_h_

namespace kernel
{
    class LocationVisitor_ABC;
}

// =============================================================================
/** @class  MoveableProxy
    @brief  Moveable proxy
*/
// Created: LGY 2012-05-07
// =============================================================================
class MoveableProxy : public kernel::Moveable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MoveableProxy( kernel::Moveable_ABC& moveable );
    virtual ~MoveableProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& position );
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Moveable_ABC* moveable_;
    //@}
};

#endif // __MoveableProxy_h_
