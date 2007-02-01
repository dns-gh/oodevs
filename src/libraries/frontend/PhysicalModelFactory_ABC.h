// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PhysicalModelFactory_ABC_h_
#define __PhysicalModelFactory_ABC_h_

#include "Messages.h"

namespace frontend
{
    class PhysicalModel;

// =============================================================================
/** @class  PhysicalModelFactory_ABC
    @brief  PhysicalModelFactory_ABC
*/
// Created: SBO 2007-01-29
// =============================================================================
class PhysicalModelFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PhysicalModelFactory_ABC() {}
    virtual ~PhysicalModelFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual PhysicalModel* Create( const ASN1T_MsgPhysicalModelCreation& message ) = 0;
    //@}
};

}

#endif // __PhysicalModelFactory_ABC_h_
