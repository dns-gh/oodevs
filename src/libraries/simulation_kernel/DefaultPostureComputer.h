// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultPostureComputer_h_
#define __DefaultPostureComputer_h_

#include "simulation_kernel/PostureComputer_ABC.h"

namespace posture
{

// =============================================================================
/** @class  DefaultPostureComputer
    @brief  DefaultPostureComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultPostureComputer : public PostureComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultPostureComputer();
    virtual ~DefaultPostureComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( const PHY_Posture& posture, bool bIsLoaded, bool bDiscreteModeEnabled );
    virtual void SetPostureMovement();
    virtual void UnsetPostureMovement();
    virtual bool MustBeForce();
    virtual const PHY_Posture& GetPosture();
    //@}

private:
    //! @name Attribute
    //@{
    bool bIsLoaded_;
    bool bDiscreteModeEnabled_;
    bool bMustBeForce_;
    const PHY_Posture* pCurrentPosture_;
    const PHY_Posture* pNewPosture_;
    //@}

};

} // namespace firing

#endif // __DefaultPostureComputer_h_
