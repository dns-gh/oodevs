// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "TerrainList.h"

// -----------------------------------------------------------------------------
// Name: TerrainList constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
TerrainList::TerrainList( QWidget* parent, kernel::Controllers& controllers )
    : ElementListView< frontend::Terrain >( parent, controllers )
{
    addColumn( tr( "Terrains" ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainList destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
TerrainList::~TerrainList()
{
    // NOTHING
}
