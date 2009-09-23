// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultDetectionComputer_h_
#define __DefaultDetectionComputer_h_

#include "simulation_kernel/DetectionComputer_ABC.h"

namespace detection
{

// =============================================================================
/** @class  DefaultDetectionComputer
    @brief  DefaultDetectionComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultDetectionComputer : public DetectionComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultDetectionComputer();
    virtual ~DefaultDetectionComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( MIL_Agent_ABC& target );
    virtual const MIL_Agent_ABC& GetTarget();

    virtual void AlreadyPerceived();
    virtual void NotifyStealth();

    virtual bool CanBeSeen();
    //@}

private:
    //! @name Attribute
    //@{
    const MIL_Agent_ABC* pTarget_;

    bool bAlreadySeen_;
    bool bIsStealth_;
    //@}

};

} // namespace detection

#endif // __DefaultDetectionComputer_h_
