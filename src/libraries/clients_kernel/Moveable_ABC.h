// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Moveable_ABC_h_
#define __Moveable_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  Moveable_ABC
    @brief  Moveable_ABC
*/
// Created: SBO 2009-02-02
// =============================================================================
class Moveable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Moveable_ABC() {}
    virtual ~Moveable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Move( const geometry::Point2f& position ) = 0;
    //@}
};

}

#endif // __Moveable_ABC_h_
