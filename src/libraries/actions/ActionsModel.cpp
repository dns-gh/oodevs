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
    , defaultPublisher_( defaultPublisher )
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
void ActionsModel::PublishAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected,
                                                 tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, const Time_ABC& simulation )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAutomatCreationAction( type, selected, point, agentsModel, agentMessenger, *this, simulation ) );
    Publish( *action, clock() );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
void ActionsModel::PublishAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected, bool force )
{
    std::unique_ptr< Action_ABC > action( factory_.CreateAgentCreationAction( type, point, selected ) );
    if( force )
        PublishForce( *action );
    else
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
    try
    {
        std::auto_ptr< Action_ABC > action( factory_.CreateAction( xis, readonly ) );
        Register( action->GetId(), *action );
        action.release();
    }
    catch( const std::exception& /*e*/ )
    {
        try
        {
            std::auto_ptr< Action_ABC > action( factory_.CreateStubAction( xis ) );
            if( action.get() )
            {
                Register( action->GetId(), *action );
                action.release();
            }
        }
        catch( const std::exception& )
        {
            // NOTHING
        }
    }
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
// Name: ActionsModel::PublishForce
// Created: HBD 2011-01-19
// -----------------------------------------------------------------------------
void ActionsModel::PublishForce( const Action_ABC& action )
{
    action.Publish( defaultPublisher_, 0 );
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
