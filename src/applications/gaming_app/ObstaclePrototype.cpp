// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObstaclePrototype.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"
#include "clients_kernel/Time_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList, const kernel::Time_ABC& simulation )
    : ObstaclePrototype_ABC( parent )
    , attributesList_( attributesList )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstaclePrototype::~ObstaclePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::GetCreationDate
// Created: JSR 2013-09-18
// -----------------------------------------------------------------------------
QDateTime ObstaclePrototype::GetCreationDate() const
{
    return simulation_.GetDateTime();
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstaclePrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Obstacle" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_obstacle );
    list.AddIdentifier( "TargetType", 0 ); // unused, kept for compatibility (deprecated preliminary/reserved type)
    list.AddBool( "Activation", IsActivated() );
    list.AddQuantity( "ActivationTime", GetActivationTime() );
    list.AddQuantity( "ActivityTime", GetActivityTime() );
}
