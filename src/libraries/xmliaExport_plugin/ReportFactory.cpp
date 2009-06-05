// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaExport_plugin_pch.h"
#include "ReportFactory.h"
#include "OrderReport.h"

using namespace plugins::xmliaExport;

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
/*
// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateOrderReport
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
OrderReport* ReportFactory::CreateOrderReport( const dispatcher::Agent& entity, const ASN1T_MsgUnitOrder& asn ) const
{
    return new OrderReport( entity, missions_, asn );
}*/

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateOrderReport
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
OrderReport* ReportFactory::CreateOrderReport( const dispatcher::Automat& entity, const ASN1T_MsgAutomatOrder& asn ) const
{
    return new OrderReport( entity, missions_, asn );
}
