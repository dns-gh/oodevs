// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_PopulationFlow_ABC_h_
#define __dispatcher_PopulationFlow_ABC_h_

#include "Sendable.h"
#include "clients_kernel/PopulationFlow_ABC.h"

namespace dispatcher
{
    class Localisation;

// =============================================================================
/** @class  PopulationFlow_ABC
    @brief  PopulationFlow_ABC
*/
// Created: SBO 2010-06-10
// =============================================================================
class PopulationFlow_ABC : public Sendable< kernel::PopulationFlow_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationFlow_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::PopulationFlow_ABC >( id, name ) {}
    virtual ~PopulationFlow_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const Localisation& GetLocation() const = 0;
    //@}
};

}

#endif // __dispatcher_PopulationFlow_ABC_h_
