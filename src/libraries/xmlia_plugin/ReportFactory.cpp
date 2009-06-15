// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ReportFactory.h"
#include "OrderReport.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const kernel::Resolver_ABC< kernel::MissionType >& missions )
    : missions_( missions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportFactory destructor
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
ReportFactory::~ReportFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateOrderReport
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
OrderReport* ReportFactory::CreateOrderReport( const dispatcher::Automat& entity, const ASN1T_MsgAutomatOrder& asn ) const
{
    return new OrderReport( entity, missions_, asn );
}
