// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultMoveComputer_h_
#define __DefaultMoveComputer_h_

#include "simulation_kernel/MoveComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  DefaultMoveComputer
    @brief  DefaultMoveComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultMoveComputer : public MoveComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultMoveComputer();
    virtual ~DefaultMoveComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void NotifyNoDotation();
    virtual void NotifyReinforcing();
    virtual void NotifyTransported();
    virtual void NotifySurrendered();
    virtual void NotifyCannotFly();
    virtual bool CanMove() const;
    //@}

private:
    //! @name Attribute
    //@{
    bool bCanMove_;
    bool isSurrendered_;
    //@}

};

} // namespace moving

#endif // __DefaultMoveComputer_h_
