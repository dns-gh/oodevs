// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Population_ABC_h_
#define __dispatcher_Population_ABC_h_

#include "Sendable.h"
#include "clients_kernel/Population_ABC.h"

namespace dispatcher
{
    class Team_ABC;
// =============================================================================
/** @class  Population_ABC
    @brief  Population_ABC
*/
// Created: SBO 2010-06-07
// =============================================================================
class Population_ABC : public Sendable< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Population_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Population_ABC >( id, name ) {}
    virtual ~Population_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual dispatcher::Team_ABC& GetTeam() const = 0;
    //@}
};

}

#endif // __dispatcher_Population_ABC_h_
