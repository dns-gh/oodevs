// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AutomatList.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::MissionParameter_AutomatList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.automatList->elem, asn.value.u.automatList->elem + asn.value.u.automatList->n, std::back_inserter( automats_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::~MissionParameter_AutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AutomatList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_automatList;
    asn.value.u.automatList = new ASN1T_AutomatList();
    asn.value.u.automatList->n = automats_.size();
    asn.value.u.automatList->elem = automats_.empty() ? 0 : (int*)&automats_.front();
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AutomatList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.automatList;
}
