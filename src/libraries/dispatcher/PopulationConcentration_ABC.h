// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_PopulationConcentration_ABC_h_
#define __dispatcher_PopulationConcentration_ABC_h_

#include "Sendable.h"
#include "clients_kernel/PopulationConcentration_ABC.h"

namespace Common
{
    class MsgCoordLatLong;
}

namespace dispatcher
{

// =============================================================================
/** @class  PopulationConcentration_ABC
    @brief  PopulationConcentration_ABC
*/
// Created: SBO 2010-06-10
// =============================================================================
class PopulationConcentration_ABC : public Sendable< kernel::PopulationConcentration_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationConcentration_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::PopulationConcentration_ABC >( id, name ) {}
    virtual ~PopulationConcentration_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const Common::MsgCoordLatLong& GetPosition() const = 0;
    //@}
};

}

#endif // __dispatcher_PopulationConcentration_ABC_h_
