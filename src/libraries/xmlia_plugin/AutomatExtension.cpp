// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "AutomatExtension.h"

#include "dispatcher/Model.h"

#include "Point.h"
#include "ReportManager.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: AutomatExtension constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
AutomatExtension::AutomatExtension( dispatcher::Automat& holder, Publisher_ABC& publisher, ReportManager& reportManager, const dispatcher::Model& model )
    : holder_( holder )
    , publisher_( publisher )
    , reportManager_( reportManager )
    , model_( model )
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
    try
    {
        reportManager_.DoUpdate( holder_, model_.GetMissionTypes().Get( message.mission ), std::vector< Point >(0), std::vector< Point >(0) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
    }
}
