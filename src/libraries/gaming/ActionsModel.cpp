// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionsModel.h"
#include "Action_ABC.h"
#include "ActionFactory_ABC.h"
#include "Simulation.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionsModel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::ActionsModel( ActionFactory_ABC& factory, const Simulation& simulation )
    : simulation_( simulation )
    , factory_( factory )
    , isRecording_( false )
    , recordingStartTime_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsModel destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::~ActionsModel()
{
    // $$$$ SBO 2007-03-12: check for unsaved actions? prompt user?
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Purge
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void ActionsModel::Purge()
{
    DeleteAll();
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
void ActionsModel::Load( const std::string& filename )
{
    std::string errors;
    xml::xifstream xis( filename );
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &ActionsModel::ReadAction, errors )
        >> xml::end();
    if( !errors.empty() )
        throw std::exception( tools::translate( "ActionsModel", "The order file contains error(s), some actions could not be loaded:\n%1" ).arg( errors.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadAction
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionsModel::ReadAction( xml::xistream& xis, std::string& errors )
{
    try
    {
        std::auto_ptr< Action_ABC > action( factory_.CreateAction( xis ) );
        Register( action->GetId(), *action );
        action.release();
    }
    catch( std::exception& e )
    {
        errors += std::string( e.what() ) + "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Save( const std::string& filename ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "actions" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "action" );
        it->second->Serialize( xos );
        xos << xml::end();
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::IsRecording
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
bool ActionsModel::IsRecording() const
{
    return isRecording_;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ToggleRecording
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::ToggleRecording()
{
    isRecording_ = !isRecording_;
}
