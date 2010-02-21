// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UnitMissionInterface.h"
#include "MissionInterfaceBuilder.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "gaming/AutomatDecisions.h"
#include "protocol/Protocol.h"

using namespace Common;

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
UnitMissionInterface::UnitMissionInterface( QWidget* parent, Entity_ABC& entity, const MissionType& mission, ActionController& controller
                                          , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : actions::gui::MissionInterface_ABC( parent, mission, entity, controller )
    , model_( model )
    , publisher_( publisher )
    , mission_( mission )
{
    builder.Build( *this, entity, mission );
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
UnitMissionInterface::~UnitMissionInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void UnitMissionInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), mission_ );
    CommitTo( *action );
    if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( GetEntity().Get< kernel::TacticalHierarchies >().GetSuperior() ) )
        if( const AutomatDecisions* decisions = automat->Retrieve< AutomatDecisions >() )
            if( decisions->IsEmbraye() )
                decisions->Disengage();
    action->Publish( publisher_ );
    if( ! model_.IsRecording() )
        model_.Destroy( *action );
}
