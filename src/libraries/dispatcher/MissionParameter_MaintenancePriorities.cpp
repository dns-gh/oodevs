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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::MissionParameter_MaintenancePriorities( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.logMaintenancePriorities->n; ++i )
        maintenancePriorities_.push_back( asn.value.u.logMaintenancePriorities->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MaintenancePriorities::~MissionParameter_MaintenancePriorities()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MaintenancePriorities::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                   = bNullValue_;
    asn.value.t                      = T_MissionParameter_value_logMaintenancePriorities;
    asn.value.u.logMaintenancePriorities = new ASN1T_LogMaintenancePriorities();
    SendContainerValues< ASN1T_LogMaintenancePriorities, ASN1T_EquipmentType, T_OIDVector >( maintenancePriorities_, *asn.value.u.logMaintenancePriorities ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MaintenancePriorities::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MaintenancePriorities::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMaintenancePriorities->n > 0 )
        delete [] asn.value.u.logMaintenancePriorities->elem;
    delete asn.value.u.logMaintenancePriorities;
}
