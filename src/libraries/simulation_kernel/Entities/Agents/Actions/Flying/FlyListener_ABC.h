// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FlyListener_ABC_h_
#define __FlyListener_ABC_h_

// =============================================================================
/** @class  FlyListener_ABC
    @brief  FlyListener_ABC
*/
// Created: LDC 2011-08-02
// =============================================================================
class FlyListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FlyListener_ABC() {}
    virtual ~FlyListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void TakeOff() = 0;
    virtual void Land() = 0;
    //@}
};

#endif // __FlyListener_ABC_h_
