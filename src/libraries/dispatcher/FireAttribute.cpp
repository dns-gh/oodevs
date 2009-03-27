// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "FireAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC   ( model, asnMsg )
    , heat_                 ( asnMsg.fire.heat )
    , nFireClass_           ( asnMsg.fire.class_id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: FireAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void FireAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if ( asnMsg.m.firePresent )
    {
        heat_       = asnMsg.fire.heat;
        nFireClass_ = asnMsg.fire.class_id;
    }  
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void FireAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.firePresent = 1;
    asnMsg.fire.heat     = heat_;
    asnMsg.fire.class_id = nFireClass_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::AsnDelete
// Created: RFT 2006-09-28
// -----------------------------------------------------------------------------
void FireAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
