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
    explicit DefaultDetectionComputer( MIL_Agent_ABC& target );
    virtual ~DefaultDetectionComputer();
    //@}

    //! @name Operations
    //@{
    virtual const MIL_Agent_ABC& GetTarget();

    virtual void AlreadyPerceived();
    virtual void NotifyStealth();
    virtual void SetUnderground( bool underground );

    virtual bool CanBeSeen();
    //@}

private:
    //! @name Attribute
    //@{
    const MIL_Agent_ABC* pTarget_;

    bool bAlreadySeen_;
    bool bIsStealth_;
    bool bIsUnderground_;
    //@}

};

} // namespace detection

#endif // __DefaultDetectionComputer_h_
