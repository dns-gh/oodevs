// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Inhabitant_ABC_h_
#define __dispatcher_Inhabitant_ABC_h_

#include "Sendable.h"
#include "clients_kernel/Inhabitant_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  Inhabitant_ABC
    @brief  Inhabitant_ABC
*/
// Created: SLG 2010-11-29
// =============================================================================
class Inhabitant_ABC : public Sendable< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Inhabitant_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Inhabitant_ABC >( id, name ) {}
    virtual ~Inhabitant_ABC() {}
    //@}
};

}

#endif // __dispatcher_Inhabitant_ABC_h_
