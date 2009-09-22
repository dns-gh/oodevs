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

#include "simulation_kernel/PostureComputer_ABC.h"
class PHY_Posture;
class PHY_UnitType;
namespace posture
{
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
             PostureComputerFactory_ABC();
    virtual ~PostureComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual PostureComputer_ABC& Create( PostureComputer_ABC::Parameters& params ) const = 0;
    //@}
};

}

#endif // __PostureComputerFactory_ABC_h_
