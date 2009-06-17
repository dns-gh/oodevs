// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "AgentExtension.h"

#include "RapportManager.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include <xeumeuleu/xml.h>


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, Publisher_ABC& publisher, RapportManager& rapportManager, const Simulation& simulation, const dispatcher::Model& model )
    : holder_( holder )
    , publisher_( publisher )
    , rapportManager_( rapportManager )
    , simulation_( simulation )
    , model_( model )
    , lastUpdate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitAttributes& attributes )
{
    //@move sort in rapport manager depending to report type
    const bool reportPosition = ( attributes.m.positionPresent || attributes.m.hauteurPresent ) && simulation_.MustReportPosition( lastUpdate_ );
    const bool reportStatus   = ( attributes.m.etat_operationnelPresent || attributes.m.dotation_eff_materielPresent || attributes.m.dotation_eff_personnelPresent ) && simulation_.MustReportStatus( lastUpdate_ );
    if( reportPosition || reportStatus )
    {
        try
        {
          rapportManager_.DoUpdate( holder_ );
        }
        catch( std::exception& e )
        {
          MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
        }      
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitDetection& message )
{
    try
    {
      rapportManager_.DoUpdate( holder_, model_.agents_.Get( message.detected_unit_oid ) );
    }
    catch( std::exception& e )
    {
      MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
    }
}
