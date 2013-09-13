// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PostureComputerFactory_h_
#define __PostureComputerFactory_h_

#include "simulation_kernel/PostureComputerFactory_ABC.h"

namespace posture
{
// =============================================================================
/** @class  PostureComputerFactory
    @brief  PostureComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class PostureComputerFactory : public PostureComputerFactory_ABC
{
public:
    //! @name ConstructorsDestructor
    //@{
             PostureComputerFactory();
    virtual ~PostureComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< PostureComputer_ABC > Create( const MIL_Random_ABC& random, const PostureTime_ABC& time, const PHY_Posture& previousPosture, const PHY_Posture& currentPosture,
                                                         bool bIsDead, bool bDiscreteModeEnabled, double rCompletionPercentage, double rStealthFactor,
                                                         double rTimingFactor, bool isParkedOnEngineerArea ) const;
    //@}
};

}

#endif // __PostureComputerFactory_h_
