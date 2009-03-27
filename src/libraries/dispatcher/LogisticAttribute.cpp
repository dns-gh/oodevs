// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "LogisticAttribute.h"

#include "Model.h"
#include "Automat.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const Model& model, const ASN1T_ObjectAttributes& asnMsg )
    : ObjectAttribute_ABC( model, asnMsg )
    , model_             ( model )
    , pTC2_              ( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    // NOTHING
}  

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogisticAttribute::Update( const ASN1T_ObjectAttributes& asnMsg )
{
    if ( asnMsg.m.logisticPresent )
        pTC2_ = &model_.automats_.Get( asnMsg.logistic.tc2 );    
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticAttribute::Send( ASN1T_ObjectAttributes& asnMsg ) const
{
    assert( pTC2_ );
    
    asnMsg.m.logisticPresent = 1;
    asnMsg.logistic.tc2 = pTC2_->GetId();     
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogisticAttribute::AsnDelete( ASN1T_ObjectAttributes& /*asnMsg*/ ) const
{
    // NOTHING
}
