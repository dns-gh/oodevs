 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceEquipmentAvailability_h_
#define __MaintenanceEquipmentAvailability_h_

namespace sword
{
    class LogMaintenanceEquipmentAvailability;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  MaintenanceEquipmentAvailability
    @brief  MaintenanceEquipmentAvailability
*/
// Created: NLD 2006-09-19
// =============================================================================
class MaintenanceEquipmentAvailability
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MaintenanceEquipmentAvailability( const sword::LogMaintenanceEquipmentAvailability& asn );
    virtual ~MaintenanceEquipmentAvailability();
    //@}

    //! @name Operations
    //@{
    void Send( sword::LogMaintenanceEquipmentAvailability& asn ) const;
    //@}

private:
    unsigned int nEquipmentType_;
    unsigned int nNbrTotal_;
    unsigned int nNbrAvailable_;
    unsigned int nNbrWorking_;
    unsigned int nNbrLent_;
    unsigned int nNbrResting_;
};

}

#endif // __MaintenanceEquipmentAvailability_h_