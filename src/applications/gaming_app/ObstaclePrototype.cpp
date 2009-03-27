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
#include "game_asn/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstaclePrototype::ObstaclePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
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

}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstaclePrototype::Commit()
{

}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype::Clean
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObstaclePrototype::Clean()
{

}
