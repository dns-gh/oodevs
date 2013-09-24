// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PostureComputerFactory_ABC_h_
#define __PostureComputerFactory_ABC_h_

#include <memory>

class PHY_Posture;
class MIL_Random_ABC;

namespace posture
{
    class PostureComputer_ABC;
    class PostureTime_ABC;

// =============================================================================
/** @class  PostureComputerFactory_ABC
    @brief  PostureComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class PostureComputerFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PostureComputerFactory_ABC() {}
    virtual ~PostureComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< PostureComputer_ABC > Create( const MIL_Random_ABC& random, const PostureTime_ABC& time, 
                                                         const PHY_Posture& previousPosture, const PHY_Posture& currentPosture,
                                                         bool bIsDead, bool bDiscreteModeEnabled, double rCompletionPercentage,
                                                         double rStealthFactor, double rTimingFactor, bool isParkedOnEngineerArea ) const = 0;
    //@}
};

}

#endif // __PostureComputerFactory_ABC_h_
