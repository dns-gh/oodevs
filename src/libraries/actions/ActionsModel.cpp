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
ActionsModel::ActionsModel( ActionFactory_ABC& factory, Publisher_ABC& publisher )
    : factory_( factory )
    , publisher_( publisher )
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
void ActionsModel::Purge( const ActionsFilter_ABC* filter /*= 0*/ )
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
Action_ABC* ActionsModel::CreateAutomatCreationAction( const kernel::AutomatType& type, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes )
{
    Action_ABC* action = factory_.CreateAutomatCreationAction( type, selected, controller, agentTypes );
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
void ActionsModel::Load( const std::string& filename, bool readonly /*= false*/ )
{
    std::string errors;
    xsl::xstringtransform xst( "resources/ordCompatibility.xsl" );
    xst << xml::xifstream( filename );
    std::string updatedFile = xst.str();
    xml::xistringstream xis( updatedFile );
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &ActionsModel::ReadAction, readonly, errors )
        >> xml::end;
    if( !errors.empty() )
        throw std::exception( tools::translate( "ActionsModel", "The order file contains error(s), some actions could not be loaded:\n%1" ).arg( errors.c_str() ) );
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
void ActionsModel::Save( const std::string& filename, const ActionsFilter_ABC* filter /*= 0*/ ) const
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
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Publish
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
void ActionsModel::Publish( const Action_ABC& action )
{
    action.Publish( publisher_ );
}
