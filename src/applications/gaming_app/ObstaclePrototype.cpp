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

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& msg )
    : ObstaclePrototype_ABC( parent )
    , msg_( msg )
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
        msg_.mutable_attributes()->mutable_obstacle()->set_type( Common::ObstacleType_DemolitionTargetType( types_->GetValue() ) );
        msg_.mutable_attributes()->mutable_obstacle()->set_activated( IsActivated() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Clean
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObstaclePrototype::Clean()
{
    if( msg_.attributes().has_obstacle() )
        msg_.mutable_attributes()->clear_obstacle();
}
