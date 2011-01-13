// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "MaintenanceEquipmentAvailability.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MaintenanceEquipmentAvailability constructor
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
MaintenanceEquipmentAvailability::MaintenanceEquipmentAvailability( const sword::LogMaintenanceEquipmentAvailability& asn )
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
// Name: MaintenanceEquipmentAvailability destructor
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
MaintenanceEquipmentAvailability::~MaintenanceEquipmentAvailability()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceEquipmentAvailability::Send
// Created: LGY 2010-07-07
// -----------------------------------------------------------------------------
void MaintenanceEquipmentAvailability::Send( sword::LogMaintenanceEquipmentAvailability& asn ) const
{
    asn.mutable_equipment()->set_id( nEquipmentType_ );
    asn.set_total( nNbrTotal_ );
    asn.set_available( nNbrAvailable_ );
    asn.set_working( nNbrWorking_ );
    asn.set_lent( nNbrLent_ );
    if( nNbrResting_ != std::numeric_limits< unsigned int >::max() )
        asn.set_resting( nNbrResting_ );
}
