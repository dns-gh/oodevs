// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "BypassAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: BypassAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
BypassAttribute::BypassAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , nPercentageBypassing_ ( asnMsg.bypass.percentage )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
BypassAttribute::~BypassAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void BypassAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if( asnMsg.m.bypassPresent )
        nPercentageBypassing_ = asnMsg.bypass.percentage;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void BypassAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.bypassPresent = 1;
    asnMsg.bypass.m.percentagePresent = 1;
    asnMsg.bypass.percentage = nPercentageBypassing_;
}

// -----------------------------------------------------------------------------
// Name: BypassAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void BypassAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{

}
