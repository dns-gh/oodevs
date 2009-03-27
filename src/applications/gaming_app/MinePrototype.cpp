// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MinePrototype.h"
#include "game_asn/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: MinePrototype constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::MinePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg )
    : MinePrototype_ABC( parent )
    , msg_ ( msg )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
MinePrototype::~MinePrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Commit
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Commit()
{
	// NOTHING
}

// -----------------------------------------------------------------------------
// Name: MinePrototype::Clean
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void MinePrototype::Clean()
{
    
}
