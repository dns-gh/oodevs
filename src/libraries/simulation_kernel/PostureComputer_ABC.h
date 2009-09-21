// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PostureComputer_ABC_h_
#define __PostureComputer_ABC_h_

class PHY_Posture;
namespace posture
{

// =============================================================================
/** @class  PostureComputer_ABC
    @brief  PostureComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class PostureComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PostureComputer_ABC();
    virtual ~PostureComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( const PHY_Posture& posture, bool bIsLoaded, bool bDiscreteModeEnabled ) = 0;
    virtual void SetPostureMovement() = 0;
    virtual void UnsetPostureMovement() = 0;
    virtual bool MustBeForce() = 0;
    virtual const PHY_Posture& GetPosture() = 0;
    //@}
};

} // namespace firing

#endif // __PostureComputer_ABC_h_
