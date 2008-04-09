// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Movable_ABC_h_
#define __Movable_ABC_h_

#include <geometry/Types.h>

namespace tic
{

// =============================================================================
/** @class  Movable_ABC
    @brief  Movable definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class Movable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Movable_ABC() {};
    virtual ~Movable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& to ) = 0;
    virtual void Stop() = 0;
    //@}
};

}

#endif // __Movable_ABC_h_
