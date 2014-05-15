// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionsModel.h"
#include "ActionsFilter_ABC.h"
#include "ActionPublisher.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/Loader_ABC.h"
#include "tools/FileWrapper.h"
#include "tools/SchemaWriter.h"
#include <tools/Path.h>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionsModel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::ActionsModel( ActionFactory_ABC& factory,
                            Publisher_ABC& defaultPublisher,
                            Controllers& controllers,
                            const Time_ABC& simulation )
    : factory_( factory )
    , controller_( controllers.controller_ )
    , publisher_( new ActionPublisher( defaultPublisher, controllers, simulation ) )
    , context_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsModel destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::~ActionsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Purge
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void ActionsModel::Purge( const ActionsFilter_ABC* filter /* = 0*/ )
{
    if( !filter )
        DeleteAll();
    else
    {
        boost::ptr_vector< actions::Action_ABC > toDestroy;
        for( IT_Elements it = elements_.begin(); it != elements_.end();  )
            if( filter->Allows( *it->second ) )
            {
                actions::Action_ABC* action = it->second;
                toDestroy.push_back( action );
                ++it;
                elements_.erase( action->GetId() );
            }
            else
                ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishAutomatCreationAction
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
int ActionsModel::PublishAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAutomatCreationAction( type, selected, point ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
int ActionsModel::PublishAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAgentCreationAction( type, point, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
int ActionsModel::PublishFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateFormationCreationAction( level, selected, isLogisticBase ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCrowdCreationAction
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
int ActionsModel::PublishCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateCrowdCreationAction( type, numberHealthy, numberWounded, numberDead, point, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
int ActionsModel::PublishCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateCrowdChangeHealthStateAction( healthy, wounded, contaminated, dead, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
int ActionsModel::PublishInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeHealthStateAction( healthy, wounded, dead, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeAlertedStateAction( alerted, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeConfinedStateAction( confined, selected ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
int ActionsModel::PublishObjectUpdateMagicAction( const kernel::Entity_ABC& object, const std::vector< parameters::ParameterList* >& attributes )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateObjectUpdateMagicAction( object, attributes ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishObjectDestroyMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
int ActionsModel::PublishObjectDestroyMagicAction( const kernel::Entity_ABC& object )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateObjectDestroyMagicAction( object ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishLogMaintenanceSetManualAction
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
int ActionsModel::PublishLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateLogMaintenanceSetManualAction( tasker, manual ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishLogSupplySetManual
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishLogSupplySetManual( const kernel::Entity_ABC& tasker, bool manual )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateLogSupplySetManual( tasker, manual ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectNewLogisticState
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
int ActionsModel::PublishSelectNewLogisticState( unsigned int consignId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectNewLogisticState( consignId ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishTransferToLogisticSuperior
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
int ActionsModel::PublishTransferToLogisticSuperior( unsigned int consignId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateTransferToLogisticSuperior( consignId ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectMaintenanceTransporter
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
int ActionsModel::PublishSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId, const boost::optional< unsigned int >& destination )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectMaintenanceTransporter( consignId, equipmentTypeId, destination ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectMaintenanceDiagnosisTeam
// Created: SLI 2014-02-06
// -----------------------------------------------------------------------------
int ActionsModel::PublishSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectMaintenanceDiagnosisTeam( consignId, equipmentTypeId ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeDiplomacy
// Created: ABR 2014-02-07
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeDiplomacy( team1, team2, diplomacy ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCreateKnowledgeGroup
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishCreateKnowledgeGroup( unsigned int id, const std::string& type )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateKnowledgeGroup( id, type ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishFireOrderOnLocation
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateFireOrderOnLocation( resourceId, location, interventionType ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeResourceLinks
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangeResourceLinks( unsigned int id, const std::map< std::string, ::gui::ResourceNode >& resourceNodes )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeResourceLinks( id, resourceNodes ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishGlobalWeather
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishGlobalWeather( const gui::WeatherParameters& params )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateGlobalWeather( params ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishLocalWeather
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishLocalWeather( const gui::LocalWeatherParameters& params )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateLocalWeather( params ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishLocalDestruction
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishLocalDestruction( unsigned int weatherId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateLocalDestruction( weatherId ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectMaintenanceRepairTeam
// Created: BAX 2014-02-10
// -----------------------------------------------------------------------------
int ActionsModel::PublishSelectMaintenanceRepairTeam( unsigned int consignId, unsigned int equipmentTypeId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectMaintenanceRepairTeam( consignId, equipmentTypeId ) );
    return Publish( *action );
}

int ActionsModel::PublishCreatePathfind( const kernel::Entity_ABC& entity, const std::vector< geometry::Point2f >& points )
{
    std::unique_ptr< Action_ABC > action( factory_.CreatePathfindCreation( entity, points ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Destroy
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ActionsModel::Destroy( const Action_ABC& action )
{
    Action_ABC* toDeleteAction = Remove( action.GetId() );
    if( toDeleteAction )
    {
        controller_.Delete( *toDeleteAction );
        delete toDeleteAction;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Load
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Load( const tools::Path& filename, const tools::Loader_ABC& fileLoader )
{
    const auto readaction = [&]( xml::xistream& xis ){
        std::auto_ptr< Action_ABC > action( factory_.CreateAction( xis ) );
        if( !action.get() )
            return;
        Register( action->GetId(), *action );
        action.release();
    };
    fileLoader.LoadFile( filename, [&]( xml::xistream& xis ){
        xis >> xml::start( "actions" )
            >> xml::list( "action", readaction )
            >> xml::end;
    });
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Save( const tools::Path& filename, const ActionsFilter_ABC* filter /* = 0*/ ) const
{
    tools::Xofstream xos( filename );
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "actions" );
    schemaWriter.WriteExerciseSchema( xos, "orders" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( !filter || filter->Allows( *it->second ) )
        {
            xos << xml::start( "action" );
            it->second->Serialize( xos );
            xos << xml::end;
        }
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Publish
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
int ActionsModel::Publish( const Action_ABC& action )
{
    action.Publish( *publisher_, ++context_ );
    return context_;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::RegisterHandler
// Created: LGY 2014-01-06
// -----------------------------------------------------------------------------
void ActionsModel::RegisterHandler( Publisher_ABC::T_SimHandler handler )
{
    publisher_->Register( handler );
}
