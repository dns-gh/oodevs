// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanLocationComputer_ABC_h_
#define __UrbanLocationComputer_ABC_h_

class PHY_Posture;

namespace urbanLocation
{
// =============================================================================
/** @class  UrbanLocationComputer_ABC
    @brief  UrbanLocationComputer_ABC
*/
// Created: SLG 2010-04-12
// =============================================================================
class UrbanLocationComputer_ABC
{

public:

    //! @name Constructors/Destructor
    //@{
             UrbanLocationComputer_ABC(){}
    virtual ~UrbanLocationComputer_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void ComputeUrbanDeployment( const PHY_Posture& currentPosture ) = 0;
    virtual float Result() = 0;
    //@}
};

} // namespace urbanLocation

#endif // __UrbanLocationComputer_ABC_h_
