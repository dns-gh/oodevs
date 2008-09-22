// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_MaintenancePriorities.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::MissionParameter_MaintenancePriorities( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.logMaintenancePriorities->elem, asn.value.u.logMaintenancePriorities->elem + asn.value.u.logMaintenancePriorities->n
             , std::back_inserter( maintenancePriorities_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::~MissionParameter_MaintenancePriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MaintenancePriorities::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_logMaintenancePriorities;
    asn.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    {
        asn.value.u.logMaintenancePriorities->n = maintenancePriorities_.size();
        asn.value.u.logMaintenancePriorities->elem = (ASN1T_EquipmentType*)( maintenancePriorities_.empty() ? 0 : &maintenancePriorities_.front() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MaintenancePriorities::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.logMaintenancePriorities;
}
