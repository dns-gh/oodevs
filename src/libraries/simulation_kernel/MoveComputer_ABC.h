// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MoveComputer_ABC_h_
#define __MoveComputer_ABC_h_

namespace moving
{

// =============================================================================
/** @class  MoveComputer_ABC
    @brief  MoveComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class MoveComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MoveComputer_ABC();
    virtual ~MoveComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset() = 0;
    virtual void NotifyReinforcing() =0;
    virtual void NotifyTransported() =0;
    virtual void NotifySurrendered() =0;
    virtual void NotifyCannotFly() =0;
    virtual bool CanMove() const = 0;
    virtual bool CanMoveOverride() const = 0;
    //@}
};

} // namespace moving

#endif // __MoveComputer_ABC_h_

