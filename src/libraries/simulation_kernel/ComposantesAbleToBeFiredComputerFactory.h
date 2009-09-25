// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ComposantesAbleToBeFiredComputerFactory_h_
#define __ComposantesAbleToBeFiredComputerFactory_h_

#include "simulation_kernel/ComposantesAbleToBeFiredComputerFactory_ABC.h"

namespace firing
{

// =============================================================================
/** @class  ComposantesAbleToBeFiredComputerFactory
    @brief  ComposantesAbleToBeFiredComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class ComposantesAbleToBeFiredComputerFactory : public ComposantesAbleToBeFiredComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             ComposantesAbleToBeFiredComputerFactory();
    virtual ~ComposantesAbleToBeFiredComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual ComposantesAbleToBeFiredComputer_ABC& Create( bool bFireOnlyOnMajorComposantes = false ) const;
    //@}
private:
    std::auto_ptr< ComposantesAbleToBeFiredComputer_ABC > pComputer_;
};

}

#endif // __ComposantesAbleToBeFiredComputerFactory_h_
