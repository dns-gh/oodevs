// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanLocationComputerFactory_ABC_h_
#define __UrbanLocationComputerFactory_ABC_h_

#include "simulation_kernel/UrbanLocationComputer_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}

namespace urbanLocation
{
// =============================================================================
/** @class  UrbanLocationComputerFactory_ABC
    @brief  UrbanLocationComputerFactory_ABC
*/
// Created: SLG 2010-04-12
// =============================================================================
class UrbanLocationComputerFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanLocationComputerFactory_ABC(){}
    virtual ~UrbanLocationComputerFactory_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< UrbanLocationComputer_ABC > Create() const = 0;
    //@}
};

}

#endif // __UrbanLocationComputerFactory_ABC_h_
