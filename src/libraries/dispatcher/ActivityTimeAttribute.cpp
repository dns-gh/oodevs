// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ActivityTimeAttribute.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ActivityTimeAttribute::ActivityTimeAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )    
    , nActivityTime_  ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ActivityTimeAttribute::~ActivityTimeAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{    
    if( asnMsg.m.activity_timePresent )
    {        
        nActivityTime_ = asnMsg.activity_time.value;    
    }
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    asnMsg.m.activity_timePresent = 1;
    asnMsg.activity_time.value = nActivityTime_;
}

// -----------------------------------------------------------------------------
// Name: ActivityTimeAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ActivityTimeAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
//    delete asnMsg.u.mine_jam;
}
