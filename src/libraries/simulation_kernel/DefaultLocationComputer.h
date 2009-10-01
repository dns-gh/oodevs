// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultLocationComputer_h_
#define __DefaultLocationComputer_h_

#include "simulation_kernel/LocationComputer_ABC.h"

namespace location
{

// =============================================================================
/** @class  DefaultLocationComputer
    @brief  DefaultLocationComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultLocationComputer : public LocationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultLocationComputer();
    virtual ~DefaultLocationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void SetHeight( double height );
    virtual double GetHeight() const;
    //@}

private:
    //! @name Attribute
    //@{
    double height_;
    //@}

};

} // namespace firing

#endif // __DefaultLocationComputer_h_
