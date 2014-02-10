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
void ActionsModel::PublishAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAutomatCreationAction( type, selected, point ) );
    Publish( *action, clock() );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
void ActionsModel::PublishAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAgentCreationAction( type, point, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
void ActionsModel::PublishFormationCreationAction( int level, const kernel::Entity_ABC& selected, bool isLogisticBase )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateFormationCreationAction( level, selected, isLogisticBase ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCrowdCreationAction
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
void ActionsModel::PublishCrowdCreationAction( const kernel::PopulationType& type, int numberHealthy, int numberWounded, int numberDead, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateCrowdCreationAction( type, numberHealthy, numberWounded, numberDead, point, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void ActionsModel::PublishCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateCrowdChangeHealthStateAction( healthy, wounded, contaminated, dead, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
void  ActionsModel::PublishInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeHealthStateAction( healthy, wounded, dead, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
void ActionsModel::PublishInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeAlertedStateAction( alerted, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
void ActionsModel::PublishInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateInhabitantChangeConfinedStateAction( confined, selected ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishObjectMagicAction
// Created: BCI 2011-01-10
// -----------------------------------------------------------------------------
void ActionsModel::PublishObjectMagicAction( const std::string& magicAction, unsigned long targetId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateObjectMagicAction( magicAction, targetId ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ActionsModel::PublishObjectUpdateMagicAction( const kernel::Entity_ABC& object, parameters::ParameterList& attribute )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateObjectUpdateMagicAction( object, attribute ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishObjectDestroyMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ActionsModel::PublishObjectDestroyMagicAction( const kernel::Entity_ABC& object )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateObjectDestroyMagicAction( object ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishLogMaintenanceSetManualAction
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void ActionsModel::PublishLogMaintenanceSetManualAction( const kernel::Entity_ABC& tasker, bool manual )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateLogMaintenanceSetManualAction( tasker, manual ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectNewLogisticState
// Created: LGY 2014-01-24
// -----------------------------------------------------------------------------
void ActionsModel::PublishSelectNewLogisticState( unsigned int consignId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectNewLogisticState( consignId ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishTransferToLogisticSuperior
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
void ActionsModel::PublishTransferToLogisticSuperior( unsigned int consignId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateTransferToLogisticSuperior( consignId ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectMaintenanceTransporter
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
void ActionsModel::PublishSelectMaintenanceTransporter( unsigned int consignId, unsigned int equipmentTypeId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectMaintenanceTransporter( consignId, equipmentTypeId ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishSelectMaintenanceDiagnosisTeam
// Created: SLI 2014-02-06
// -----------------------------------------------------------------------------
void ActionsModel::PublishSelectMaintenanceDiagnosisTeam( unsigned int consignId, unsigned int equipmentTypeId )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateSelectMaintenanceDiagnosisTeam( consignId, equipmentTypeId ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeDiplomacy
// Created: ABR 2014-02-07
// -----------------------------------------------------------------------------
void ActionsModel::PublishChangeDiplomacy( unsigned int team1, unsigned int team2, sword::EnumDiplomacy diplomacy )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeDiplomacy( team1, team2, diplomacy ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishCreateKnowledgeGroup
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
void ActionsModel::PublishCreateKnowledgeGroup( unsigned int id, const std::string& type )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateKnowledgeGroup( id, type ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishFireOrderOnLocation
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
void ActionsModel::PublishFireOrderOnLocation( unsigned int resourceId, const kernel::Location_ABC& location, float interventionType )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateFireOrderOnLocation( resourceId, location, interventionType ) );
    Publish( *action, 0 );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishChangeResourceLinks
// Created: ABR 2014-02-10
// -----------------------------------------------------------------------------
void ActionsModel::PublishChangeResourceLinks( unsigned int id, const ::gui::ResourceNetwork_ABC::T_ResourceNodes& resourceNodes )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateChangeResourceLinks( id, resourceNodes ) );
    Publish( *action, 0 );
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
void ActionsModel::Load( const tools::Path& filename, const tools::Loader_ABC& fileLoader, bool readonly /* = false*/ )
{
    fileLoader.LoadFile( filename, boost::bind( &ActionsModel::ReadActions, this, _1, readonly ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadActions
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::ReadActions( xml::xistream& xis, bool readonly )
{
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &ActionsModel::ReadAction, readonly )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadAction
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionsModel::ReadAction( xml::xistream& xis, bool readonly )
{
    std::auto_ptr< Action_ABC > action( factory_.CreateAction( xis, readonly ) );
    Register( action->GetId(), *action );
    action.release();
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
void ActionsModel::Publish( const Action_ABC& action, int context )
{
    action.Publish( *publisher_, context );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::SetTimelineHandler
// Created: ABR 2013-12-13
// -----------------------------------------------------------------------------
void ActionsModel::SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& handler )
{
    publisher_->SetTimelineHandler( handler );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::RegisterHandler
// Created: LGY 2014-01-06
// -----------------------------------------------------------------------------
void ActionsModel::RegisterHandler( Publisher_ABC::T_SimHandler handler )
{
    publisher_->Register( handler );
}
