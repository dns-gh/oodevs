// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultUrbanLocationComputer_h_
#define __DefaultUrbanLocationComputer_h_

#include "simulation_kernel/UrbanLocationComputer_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}

namespace urbanLocation
{
// =============================================================================
/** @class  DefaultUrbanLocationComputer
    @brief  DefaultUrbanLocationComputer
*/
// Created: SLG 2010-04-12
// =============================================================================
class DefaultUrbanLocationComputer : public UrbanLocationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultUrbanLocationComputer( const urban::TerrainObject_ABC& block, float urbanDeployment );
    virtual ~DefaultUrbanLocationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void ComputeUrbanDeployment( const PHY_Posture& currentPosture );
    virtual float Result();
    //@}

private:
    //! @name Attribute
    //@{
    const urban::TerrainObject_ABC& block_;
    float urbanDeployment_;
    //@}

};

} // namespace urbanLocation

#endif // __DefaultUrbanLocationComputer_h_
