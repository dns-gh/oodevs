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
#include "Action_ABC.h"
#include "ActionFactory_ABC.h"
#include "ActionsFilter_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/StaticModel.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/Loader_ABC.h"
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
ActionsModel::ActionsModel( ActionFactory_ABC& factory, Publisher_ABC& publisher, Publisher_ABC& defaultPublisher )
    : factory_         ( factory )
    , publisher_       ( publisher )
    , defaultPublisher_( defaultPublisher )
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
// Name: ActionsModel::CreateAction
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAction( const Entity_ABC& target, const MissionType& mission )
{
    Action_ABC* action = factory_.CreateAction( target, mission );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAction( const Entity_ABC& target, const FragOrderType& fragOrder )
{
    Action_ABC* action = factory_.CreateAction( target, fragOrder );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAutomatCreationAction
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected,
                                                      tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, const Time_ABC& simulation  )
{
   Action_ABC* action = factory_.CreateAutomatCreationAction( type, selected, point, agentsModel, agentMessenger, *this, simulation );
   Register( action->GetId(), *action );
   return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateAgentCreationAction( type, point, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateFormationCreationAction( level, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateCrowdCreationAction
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateCrowdCreationAction( const kernel::PopulationType& type, int number, const geometry::Point2f& point, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateCrowdCreationAction( type, number, point, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateCrowdChangeHealthStateAction( healthy, wounded, contaminated, dead, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateInhabitantChangeHealthStateAction( healthy, wounded, dead, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateInhabitantChangeAlertedStateAction( alerted, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected )
{
    Action_ABC* action = factory_.CreateInhabitantChangeConfinedStateAction( confined, selected );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateObjectMagicAction
// Created: BCI 2011-01-10
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId )
{
    Action_ABC* action = factory_.CreateObjectMagicAction( magicAction, targetId );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateObjectUpdateMagicAction( const kernel::Object_ABC& object, parameters::ParameterList& attribute )
{
    Action_ABC* action = factory_.CreateObjectUpdateMagicAction( object, attribute );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateObjectDestroyMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateObjectDestroyMagicAction( const kernel::Object_ABC& object )
{
    Action_ABC* action = factory_.CreateObjectDestroyMagicAction( object );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Destroy
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
void ActionsModel::Destroy( const Action_ABC& action )
{
    Remove( action.GetId() );
    delete &action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Load
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Load( const std::string& filename, const tools::Loader_ABC& fileLoader, bool readonly /* = false*/ )
{
    fileLoader.LoadFile( filename, boost::bind( &ActionsModel::ReadActions, this, _1, readonly ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadActions
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::ReadActions( xml::xistream& xis, bool readonly )
{
    std::string errors;
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &ActionsModel::ReadAction, readonly, errors )
        >> xml::end;
//    if( !errors.empty() )
//        throw std::exception( tools::translate( "ActionsModel", "The order file contains error(s), some actions could not be loaded:\n%1" ).arg( errors.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadAction
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionsModel::ReadAction( xml::xistream& xis, bool readonly, std::string& errors )
{
    try
    {
        std::auto_ptr< Action_ABC > action( factory_.CreateAction( xis, readonly ) );
        Register( action->GetId(), *action );
        action.release();
    }
    catch( std::exception& e )
    {
        errors += std::string( e.what() ) + "\n";
        try
        {
            std::auto_ptr< Action_ABC > action( factory_.CreateStubAction( xis ) );
            if( action.get() )
            {
                Register( action->GetId(), *action );
                action.release();
            }
        }
        catch( std::exception& )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Save( const std::string& filename, const ActionsFilter_ABC* filter /* = 0*/ ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "actions" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
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
    action.Publish( publisher_, context );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::PublishForce
// Created: HBD 2011-01-19
// -----------------------------------------------------------------------------
void ActionsModel::PublishForce( const Action_ABC& action )
{
    action.Publish( defaultPublisher_ );
}
