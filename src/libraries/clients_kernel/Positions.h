// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Positions_h_
#define __Positions_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"

namespace kernel
{
    class LocationVisitor_ABC;

    // $$$$ ABR 2011-10-19: Rename to Positions_ABC, and create a new class Positions that will implement common method for each sub positions class.

// =============================================================================
/** @class  Positions
    @brief  Positions
*/
// Created: AGE 2006-03-22
// =============================================================================
class Positions : public Extension_ABC
                , public Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Positions() {}
    virtual ~Positions() {}
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregate = true ) const = 0;
    virtual float GetHeight( bool aggregate = true ) const = 0;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const = 0;
    virtual geometry::Rectangle2f GetBoundingBox() const = 0;
    virtual void Accept( LocationVisitor_ABC& visitor ) const = 0;
    virtual bool CanAggregate() const = 0;
    virtual void Compute() {}
    //@}
};

}

#endif // __Positions_h_
