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

#include "ReportManager.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>


using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, Publisher_ABC& publisher, ReportManager& reportManager, const Simulation& simulation, const dispatcher::Model& model )
    : holder_( holder )
    , publisher_( publisher )
    , reportManager_( reportManager )
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
          reportManager_.DoUpdate( holder_ );
        }
        catch( std::exception& e )
        {
          MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
        }      
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitDetection& message )
{
    try
    {
      reportManager_.DoUpdate( holder_, model_.agents_.Get( message.detected_unit_oid ) );
    }
    catch( std::exception& e )
    {
      MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgObjectDetection& message )
{
  try
  {
    reportManager_.DoUpdate( holder_, model_.objects_.Get( message.object_oid ) );
  }
  catch( std::exception& e )
  {
    MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
  }
}


// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitOrder& message )
{
   try
   {
     reportManager_.DoUpdate( holder_, model_.GetMissionTypes().Get( message.mission ) ); //@TODO ADD LIMIT
   }
   catch( std::exception& e )
   {
     MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
   }
}
// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgStartUnitFire& message )
{
  try
  {
    reportManager_.DoUpdateIntrep( holder_, model_.agents_.Get( message.fire_oid ) );
  }
  catch( std::exception& e )
  {
    MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
  }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_DotationQuery& message )
{
  try
  {
    reportManager_.DoUpdateLog( holder_ );
  }
  catch( std::exception& e )
  {
    MT_LOG_ERROR_MSG( "XMLIA error storing report information: " << e.what() );
  }
}

