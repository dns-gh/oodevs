// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MissionLayer.h"
#include "MissionPanel.h"

// -----------------------------------------------------------------------------
// Name: MissionLayer constructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
MissionLayer::MissionLayer( MissionPanel& panel )
    : panel_( panel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionLayer destructor
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
MissionLayer::~MissionLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionLayer::Paint
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionLayer::Paint( const geometry::Rectangle2f& viewport )
{
    panel_.Draw( viewport );
}
