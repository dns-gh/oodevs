// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanLocationComputerFactory_h_
#define __UrbanLocationComputerFactory_h_

#include "simulation_kernel/UrbanLocationComputerFactory_ABC.h"
#include "simulation_kernel/UrbanLocationComputer_ABC.h"

namespace urbanLocation
{
// =============================================================================
/** @class  UrbanLocationComputerFactory
    @brief  UrbanLocationComputerFactory
*/
// Created: SLG 2010-04-12
// =============================================================================
class UrbanLocationComputerFactory : public UrbanLocationComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             UrbanLocationComputerFactory();
    virtual ~UrbanLocationComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< UrbanLocationComputer_ABC > Create( const urban::TerrainObject_ABC& block, float deployment ) const;
    //@}
};

}

#endif // __UrbanLocationComputerFactory_h_
