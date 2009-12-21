// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "DotationQuota.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DotationQuota constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DotationQuota::DotationQuota( const Model_ABC& /*model*/, const ASN1T_DotationQuota& asnMsg )
   : nDotationType_( asnMsg.ressource_id )
   , nQuota_       ( asnMsg.quota_disponible )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationQuota destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DotationQuota::~DotationQuota()
{
    // NOTHING
}
       
// -----------------------------------------------------------------------------
// Name: DotationQuota::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void DotationQuota::Update( const ASN1T_DotationQuota& asnMsg )
{
    nQuota_ = asnMsg.quota_disponible;
}

// -----------------------------------------------------------------------------
// Name: DotationQuota::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void DotationQuota::Send( ASN1T_DotationQuota& asnMsg ) const
{
    asnMsg.ressource_id     = nDotationType_;
    asnMsg.quota_disponible = nQuota_;
}

