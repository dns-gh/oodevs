// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceStates_ABC_h_
#define __MaintenanceStates_ABC_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"

namespace kernel
{
    class Availability;
    class EquipmentType;

// =============================================================================
/** @class  MaintenanceStates_ABC
    @brief  MaintenanceStates_ABC
*/
// Created: LDC 2012-04-24
// =============================================================================
class MaintenanceStates_ABC : public Extension_ABC
                            , public Displayable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates_ABC() {}
    virtual ~MaintenanceStates_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const std::vector< const kernel::EquipmentType* >& GetPriorities() const = 0;
    virtual const std::vector< kernel::Availability >& GetDispoHaulers() const = 0;
    virtual const std::vector< kernel::Availability >& GetDispoRepairers() const = 0;
    virtual bool HasPriority( const kernel::EquipmentType* ) const = 0;
    virtual bool IsEnabled() const = 0;
    //@}
};

}

#endif // __MaintenanceStates_ABC_h_