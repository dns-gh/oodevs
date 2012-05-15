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

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, actions::parameters::ParameterList*& attributesList )
    : ObstaclePrototype_ABC( parent )
    , attributesList_( attributesList )
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
// Name: ObstaclePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstaclePrototype::Commit()
{
    if( types_ )
    {
        actions::parameters::ParameterList& list = attributesList_->AddList( "Obstacle" );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_obstacle );
        list.AddIdentifier( "TargetType", types_->GetValue() );
        list.AddBool( "Activation", IsActivated() );
        list.AddQuantity( "ActivationTime", GetActivationTime() );
        list.AddQuantity( "ActivityTime", GetActivityTime() );
    }
}
