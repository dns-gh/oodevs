// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "AutomatExtension.h"
#include "PositionReport.h"
#include "OrderReport.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: AutomatExtension constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
AutomatExtension::AutomatExtension( dispatcher::Automat& holder, Publisher& publisher )
    : holder_( holder )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatExtension destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
AutomatExtension::~AutomatExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatExtension::DoUpdate
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void AutomatExtension::DoUpdate( const ASN1T_MsgAutomatOrder& message )
{
    OrderReport report( holder_, message );
    report.Send( publisher_ );
}
