// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_Objective.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Objective::MissionParameter_Objective( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , data_               ( *asn.value.u.missionObjective )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_Objective::~MissionParameter_Objective()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Objective::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value               = bNullValue_;
    asn.value.t                  = T_MissionParameter_value_missionObjective;
    asn.value.u.missionObjective = new ASN1T_MissionObjective();
    data_.Send( *asn.value.u.missionObjective );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_Objective::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_Objective::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.missionObjective;
}
