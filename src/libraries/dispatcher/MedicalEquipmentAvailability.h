 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalEquipmentAvailability_h_
#define __MedicalEquipmentAvailability_h_

namespace sword
{
    class LogMedicalEquipmentAvailability;
}

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  MedicalEquipmentAvailability
    @brief  MedicalEquipmentAvailability
*/
// Created: NLD 2006-09-19
// =============================================================================
class MedicalEquipmentAvailability
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalEquipmentAvailability( const sword::LogMedicalEquipmentAvailability& asn );
    virtual ~MedicalEquipmentAvailability();
    //@}

    //! @name Operations
    //@{
    void Send( sword::LogMedicalEquipmentAvailability& asn ) const;
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

#endif // __MedicalEquipmentAvailability_h_