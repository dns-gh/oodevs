// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LocationComputerFactory_h_
#define __LocationComputerFactory_h_

#include "LocationComputerFactory_ABC.h"

namespace location
{
// =============================================================================
/** @class  LocationComputerFactory
    @brief  LocationComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class LocationComputerFactory : public LocationComputerFactory_ABC
{
public:
    //! @name ConstructorsDestructor
    //@{
             LocationComputerFactory();
    virtual ~LocationComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< LocationComputer_ABC > Create() const;
    //@}
};

}

#endif // __LocationComputerFactory_h_
