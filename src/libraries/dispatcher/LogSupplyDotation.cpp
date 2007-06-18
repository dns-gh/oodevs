// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "LogSupplyDotation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyDotation::LogSupplyDotation( const Model& /*model*/, const ASN1T_DotationQuery& asn )
    : nDotationType_( asn.ressource_id        )
    , nNbrRequested_( asn.quantite_demandee   )
    , nNbrGranted_  ( asn.quantite_accordee   )
    , nNbrConvoyed_ ( asn.quantite_en_transit )
{
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyDotation::~LogSupplyDotation()
{

}
       
// -----------------------------------------------------------------------------
// Name: LogSupplyDotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogSupplyDotation::Update( const ASN1T_DotationQuery& asn )
{
    nNbrRequested_ = asn.quantite_demandee;
    nNbrGranted_   = asn.quantite_accordee;
    nNbrConvoyed_  = asn.quantite_en_transit;
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogSupplyDotation::Send( ASN1T_DotationQuery& asn ) const
{
    asn.ressource_id        = nDotationType_;
    asn.quantite_demandee   = nNbrRequested_;
    asn.quantite_accordee   = nNbrGranted_;
    asn.quantite_en_transit = nNbrConvoyed_;
}
