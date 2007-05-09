// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_MedicalPriorities.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::MissionParameter_MedicalPriorities( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.santePriorites->n; ++i )
        medicalPriorities_.push_back( asn.value.u.santePriorites->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::~MissionParameter_MedicalPriorities()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value             = bNullValue_;
    asn.value.t                = T_MissionParameter_value_santePriorites;
    asn.value.u.santePriorites = new ASN1T_SantePriorites();
    SendContainerValues< ASN1T_SantePriorites, ASN1T_EnumHumanWound, T_OIDVector >( medicalPriorities_, *asn.value.u.santePriorites ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listAgent->n > 0 )
        delete [] asn.value.u.santePriorites->elem;
    delete asn.value.u.santePriorites;
}
