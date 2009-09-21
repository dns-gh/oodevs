// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __LocationComputerFactory_ABC_h_
#define __LocationComputerFactory_ABC_h_

namespace location
{
    class LocationComputer_ABC;
// =============================================================================
/** @class  LocationComputerFactory_ABC
    @brief  LocationComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class LocationComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LocationComputerFactory_ABC();
    virtual ~LocationComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual LocationComputer_ABC& Create() const = 0;
    //@}
};

}

#endif // __LocationComputerFactory_ABC_h_
