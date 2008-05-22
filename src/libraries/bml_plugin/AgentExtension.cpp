// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "AgentExtension.h"
#include "PositionReport.h"
#include "OrderReport.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, Publisher& publisher )
    : holder_( holder )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitAttributes& attributes )
{
    if( attributes.m.positionPresent || attributes.m.hauteurPresent )
    {
        PositionReport report( holder_ );
        report.Send( publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitOrder& message )
{
    OrderReport report( holder_, message );
    report.Send( publisher_ );
}
