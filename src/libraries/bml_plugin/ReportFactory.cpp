// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "ReportFactory.h"
#include "OrderReport.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: ReportFactory constructor
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
ReportFactory::ReportFactory( const tools::Resolver_ABC< kernel::MissionType >& missions )
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
OrderReport* ReportFactory::CreateOrderReport( const dispatcher::Agent_ABC& entity, const sword::UnitOrder& asn ) const
{
    return new OrderReport( entity, missions_, asn );
}

// -----------------------------------------------------------------------------
// Name: ReportFactory::CreateOrderReport
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
OrderReport* ReportFactory::CreateOrderReport( const dispatcher::Automat_ABC& entity, const sword::AutomatOrder& asn ) const
{
    return new OrderReport( entity, missions_, asn );
}
