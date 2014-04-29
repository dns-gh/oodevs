// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __Equipments_ABC_h_
#define __Equipments_ABC_h_

#include "Extension_ABC.h"

namespace sword
{
    class LogMaintenanceHandlingCreation;
    class LogMaintenanceHandlingUpdate;
}

namespace kernel
{
    class EquipmentType;

// =============================================================================
/** @class  Equipments_ABC
    @brief  Equipments_ABC
*/
// Created: MCO 2014-04-28
// =============================================================================
class Equipments_ABC : public kernel::Extension_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Equipments_ABC() {}
    virtual ~Equipments_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool HasEquipment( const kernel::EquipmentType& type ) const = 0;
    virtual float GetTotalWeight() const = 0;

    virtual void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message ) = 0;
    virtual void DeleteMaintenanceConsign( int id ) = 0;
    virtual void Update( const sword::LogMaintenanceHandlingUpdate& message ) = 0;
    //@}
};

} // end namespace

#endif // __Equipments_ABC_h_
