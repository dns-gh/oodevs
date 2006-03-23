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

#include "Extension_ABC.h"

// =============================================================================
/** @class  Positions
    @brief  Positions
*/
// Created: AGE 2006-03-22
// =============================================================================
class Positions : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Positions() {};
    virtual ~Positions() {};
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const = 0;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const = 0;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const = 0;
    //@}
};

#endif // __Positions_h_
