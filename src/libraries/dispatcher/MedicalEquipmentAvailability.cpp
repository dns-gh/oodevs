// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "MedicalEquipmentAvailability.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MedicalEquipmentAvailability constructor
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
MedicalEquipmentAvailability::MedicalEquipmentAvailability( const sword::LogMedicalEquipmentAvailability& asn )
: nEquipmentType_( asn.equipment().id() )
, nNbrTotal_     ( asn.total() )
, nNbrAvailable_ ( asn.available() )
, nNbrWorking_   ( asn.working() )
, nNbrLent_      ( asn.lent() )
, nNbrResting_   ( asn.has_resting() ? asn.resting() : std::numeric_limits< unsigned int >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalEquipmentAvailability destructor
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
MedicalEquipmentAvailability::~MedicalEquipmentAvailability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalEquipmentAvailability::Send
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
void MedicalEquipmentAvailability::Send( sword::LogMedicalEquipmentAvailability& asn ) const
{
    asn.mutable_equipment()->set_id( nEquipmentType_ );
    asn.set_total( nNbrTotal_ );
    asn.set_available( nNbrAvailable_ );
    asn.set_working( nNbrWorking_ );
    asn.set_lent( nNbrLent_ );
    if( nNbrResting_ != std::numeric_limits< unsigned int >::max() )
    {        
        asn.set_resting( nNbrResting_ );
    }   
}
