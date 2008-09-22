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
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::MissionParameter_MedicalPriorities( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.logMedicalPriorities->elem, asn.value.u.logMedicalPriorities->elem + asn.value.u.logMedicalPriorities->n
             , std::back_inserter( medicalPriorities_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_MedicalPriorities::~MissionParameter_MedicalPriorities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_logMedicalPriorities;
    asn.value.u.logMedicalPriorities = new ASN1T_LogMedicalPriorities();
    {
        asn.value.u.logMedicalPriorities->n = medicalPriorities_.size();
        asn.value.u.logMedicalPriorities->elem = (ASN1T_EnumHumanWound*)( medicalPriorities_.empty() ? 0 : &medicalPriorities_.front() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_MedicalPriorities::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_MedicalPriorities::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.logMedicalPriorities;
}
