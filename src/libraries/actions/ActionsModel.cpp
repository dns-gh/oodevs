// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionsModel.h"
#include "ActionPublisher.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionsModel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::ActionsModel( ActionFactory_ABC& factory,
                            Publisher_ABC& defaultPublisher,
                            kernel::Controllers& controllers,
                            const kernel::Time_ABC& simulation )
    : factory_( factory )
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
    // NOTHING
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

int ActionsModel::PublishCreatePathfind( uint32_t unit, const kernel::Entity_ABC& entity, const std::vector< geometry::Point2f >& points, const std::string& name )
{
    std::unique_ptr< Action_ABC > action( factory_.CreatePathfindCreation( unit, entity, points, name ) );
    return Publish( *action );
}

int ActionsModel::PublishUpdatePathfind( const sword::Pathfind& pathfind )
{
    std::unique_ptr< Action_ABC > action( factory_.CreatePathfindUpdate( pathfind ) );
    return Publish( *action );
}

int ActionsModel::PublishDestroyPathfind( uint32_t id )
{
    std::unique_ptr< Action_ABC > action( factory_.CreatePathfindDestruction( id ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeLogisticLinks
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangeLogisticLinks( const kernel::Entity_ABC& entity,
                                              const kernel::Entity_ABC* nominalSuperior,
                                              const kernel::Entity_ABC* currentSuperior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeLogisticLinks( entity, nominalSuperior, currentSuperior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishUnitChangeSuperior
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishUnitChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Automat_ABC& superior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateUnitChangeSuperior( entity, superior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeFormationSuperior
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangeFormationSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeFormationSuperior( entity, superior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeKnowledgeGroup
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangeKnowledgeGroup( const kernel::Automat_ABC& entity, const kernel::KnowledgeGroup_ABC& superior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeKnowledgeGroup( entity, superior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishKnowledgeGroupUpdateParty
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishKnowledgeGroupUpdateParty( const kernel::KnowledgeGroup_ABC& entity, const kernel::Team_ABC& superior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateKnowledgeGroupUpdateParty( entity, superior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishKnowledgeGroupUpdatePartyParent
// Created: ABR 2014-06-03
// -----------------------------------------------------------------------------
int ActionsModel::PublishKnowledgeGroupUpdatePartyParent( const kernel::KnowledgeGroup_ABC& entity, const kernel::KnowledgeGroup_ABC& superior )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateKnowledgeGroupUpdatePartyParent( entity, superior ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishRename
// Created: ABR 2014-08-27
// -----------------------------------------------------------------------------
int ActionsModel::PublishRename( const kernel::Entity_ABC& entity, const QString& name )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateRename( entity, name ) );
    return Publish( *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeTacticalLineSuperior
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void ActionsModel::PublishChangeTacticalLineSuperior( kernel::TacticalLine_ABC& line, const kernel::Entity_ABC& target )
{
    line.ChangeSuperior( target );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeDrawingSuperior
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
void ActionsModel::PublishChangeDrawingSuperior( kernel::Drawing_ABC& drawing, const kernel::Entity_ABC& target )
{
    drawing.ChangeSuperior( target );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangePathfindSuperior
// Created: LDC 2014-11-07
// -----------------------------------------------------------------------------
int ActionsModel::PublishChangePathfindSuperior( kernel::Pathfind_ABC& item, const kernel::Entity_ABC& target )
{
    return item.ChangeSuperior( target );
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
