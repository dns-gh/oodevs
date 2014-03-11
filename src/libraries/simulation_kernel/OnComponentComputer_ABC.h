// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __OnComponentComputer_ABC_h_
#define __OnComponentComputer_ABC_h_

class PHY_ComposantePion;

// =============================================================================
/** @class  OnComponentComputer_ABC
    @brief  OnComponentComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class OnComponentComputer_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             OnComponentComputer_ABC() {}
    virtual ~OnComponentComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComponent( PHY_ComposantePion& component ) = 0;
    //@}
};

#endif // __OnComponentComputer_ABC_h_
