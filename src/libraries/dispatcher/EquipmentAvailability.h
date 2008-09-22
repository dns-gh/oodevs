 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentAvailability_h_
#define __EquipmentAvailability_h_

#include "game_asn/Simulation.h"

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  EquipmentAvailability
    @brief  EquipmentAvailability
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T >
class EquipmentAvailability
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EquipmentAvailability( const T& asn );
    virtual ~EquipmentAvailability();
    //@}

    //! @name Operations
    //@{
    void Send( T& asn ) const;
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

#include "EquipmentAvailability.inl"

#endif // __EquipmentAvailability_h_