// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultMagicMoveComputer_h_
#define __DefaultMagicMoveComputer_h_

#include "simulation_kernel/MoveComputer_ABC.h"

namespace moving
{

// =============================================================================
/** @class  DefaultMagicMoveComputer
    @brief  DefaultMagicMoveComputer
*/
// Created: NLD 2011-11-15
// =============================================================================
class DefaultMagicMoveComputer : public MoveComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DefaultMagicMoveComputer();
    virtual ~DefaultMagicMoveComputer();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyReinforcing();
    virtual void NotifyTransported();
    virtual void NotifySurrendered();
    virtual void NotifyCannotFly();
    virtual bool CanMove() const;
    bool CanMoveOverride() const;
    //@}

private:
    //! @name Attribute
    //@{
    bool bCanMagicMove_;
    bool isSurrendered_;
    //@}
};

} // namespace moving

#endif // __DefaultMagicMoveComputer_h_
