// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "TerrainsTab.h"
#include "TerrainList.h"
#include "Panels.h"

// -----------------------------------------------------------------------------
// Name: TerrainsTab constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
TerrainsTab::TerrainsTab( QWidget* parent, kernel::Controllers& controllers )
    : QHBox( parent )
{
    setStretchFactor( new TerrainList( this, controllers ), 1 );
    setStretchFactor( new Panels( this ), 3 ); // $$$$ SBO 2007-02-01: dummy
}

// -----------------------------------------------------------------------------
// Name: TerrainsTab destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
TerrainsTab::~TerrainsTab()
{
    // NOTHING
}
