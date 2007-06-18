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
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::MissionParameter_AutomatList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.automatList->n; ++i )
        automats_.push_back( asn.value.u.automatList->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AutomatList::~MissionParameter_AutomatList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AutomatList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value           = bNullValue_;
    asn.value.t              = T_MissionParameter_value_automatList;
    asn.value.u.automatList = new ASN1T_AutomatList();
    SendContainerValues< ASN1T_AutomatList, ASN1T_Automat, T_OIDVector >( automats_, *asn.value.u.automatList ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AutomatList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AutomatList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.automatList->n > 0 )
        delete [] asn.value.u.automatList->elem;
    delete asn.value.u.automatList;
}
