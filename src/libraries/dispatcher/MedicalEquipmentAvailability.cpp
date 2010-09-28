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
MedicalEquipmentAvailability::MedicalEquipmentAvailability( const MsgsSimToClient::MsgLogMedicalEquipmentAvailability& asn )
: nEquipmentType_( asn.equipment_type().id() )
, nNbrTotal_     ( asn.nbr_total() )
, nNbrAvailable_ ( asn.nbr_disponibles() )
, nNbrWorking_   ( asn.nbr_au_travail() )
, nNbrLent_      ( asn.nbr_pretes() )
, nNbrResting_   ( asn.has_nbr_au_repos() ? asn.nbr_au_repos() : std::numeric_limits< unsigned int >::max() )
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
void MedicalEquipmentAvailability::Send( MsgsSimToClient::MsgLogMedicalEquipmentAvailability& asn ) const
{
    asn.mutable_equipment_type()->set_id( nEquipmentType_ );
    asn.set_nbr_total( nNbrTotal_ );
    asn.set_nbr_disponibles( nNbrAvailable_ );
    asn.set_nbr_au_travail( nNbrWorking_ );
    asn.set_nbr_pretes( nNbrLent_ );
    if( nNbrResting_ != std::numeric_limits< unsigned int >::max() )
    {        
        asn.set_nbr_au_repos( nNbrResting_ );
    }   
}
