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
#include "simulation_kernel/PostureComputer_ABC.h"

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
    virtual PostureComputer_ABC& Create( PostureComputer_ABC::Parameters& params ) const;
    //@}

private:
    //! @name Operations
    //@{
    std::auto_ptr< PostureComputer_ABC > pPostureComputer_;
    //@}

private:
    //! @name Attribute
    //@{
    MT_Random random_;
    //@}
};

}

#endif // __PostureComputerFactory_h_
