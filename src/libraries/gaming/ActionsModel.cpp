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
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionsModel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::ActionsModel( ActionFactory_ABC& factory )
    : factory_( factory )
    , isRecording_( false )
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
Action_ABC* ActionsModel::CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission )
{
    Action_ABC* action = factory_.CreateAction( target, mission );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder )
{
    Action_ABC* action = factory_.CreateAction( target, fragOrder );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Load
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Load( const std::string& filename )
{
    Purge();
    xml::xifstream xis( filename );
    xis >> start( "actions" )
            >> list( "mission", *this, &ActionsModel::ReadMission )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::ReadMission
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionsModel::ReadMission( xml::xistream& xis )
{
    Action_ABC* action = factory_.CreateAction( xis );
    Register( action->GetId(), *action );
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsModel::Save( const std::string& filename ) const
{
    xml::xofstream xos( filename );
    xos << start( "actions" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Serialize( xos );
    xos << end();
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
