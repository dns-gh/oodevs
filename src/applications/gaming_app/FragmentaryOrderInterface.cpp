// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FragmentaryOrderInterface.h"
#include "MissionInterfaceBuilder.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/FragOrderType.h"
#include "protocol/Protocol.h"       

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: SBO 2006-11-23
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( QWidget* parent, Entity_ABC& entity, const FragOrderType& fragOrder, ActionController& controller
                                                    , Publisher_ABC& publisher, MissionInterfaceBuilder& builder, ActionsModel& model )
    : actions::gui::MissionInterface_ABC( parent, fragOrder, entity, controller )
    , model_( model )
    , publisher_( publisher )
    , fragOrder_( fragOrder )
{
    builder.Build( *this, entity, fragOrder );
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::~FragmentaryOrderInterface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface::Publish
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::Publish()
{
    Action_ABC* action = model_.CreateAction( GetEntity(), fragOrder_ );
    CommitTo( *action );
    model_.Publish( *action, publisher_ );
}
