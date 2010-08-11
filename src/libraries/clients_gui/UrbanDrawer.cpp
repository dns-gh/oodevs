// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanDrawer.h"
#include "clients_kernel/GlTools_ABC.h"
#include <urban/TerrainObject_ABC.h>
#include <urban/Location_ABC.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::UrbanDrawer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanDrawer::UrbanDrawer( const kernel::GlTools_ABC& tools )
    : toolbox_( tools )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::~UrbanDrawer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanDrawer::~UrbanDrawer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( const urban::TerrainObject_ABC& object ) const
{
    toolbox_.DrawDecoratedPolygon( *object.GetFootprint(), object.GetDecoration() );
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( Location_ABC& /*location*/ ) const
{
    //TODO
}
