// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LocationComputer_ABC_h_
#define __LocationComputer_ABC_h_

namespace location
{
// =============================================================================
/** @class  LocationComputer_ABC
    @brief  LocationComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class LocationComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LocationComputer_ABC() {}
    virtual ~LocationComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void IncreaseHeight( double height ) = 0;
    virtual void SetFlying() = 0;
    virtual double GetHeight() const = 0;
    //@}
};

} // namespace location

#endif // __LocationComputer_ABC_h_
