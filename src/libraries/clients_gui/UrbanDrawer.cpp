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
#include "urban/TerrainObject_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "urban/Location_ABC.h"

#include "urban/Block.h"
#include "urban/City.h"
#include "urban/District.h"
#include "urban/ResourceNetwork.h"
#include "urban/UndergroundNetwork.h"

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
void UrbanDrawer::Draw( const urban::Block& object, bool selected ) const
{
    toolbox_.DrawConvexPolygon( object.GetFootprint()->Vertices(), selected );
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( const urban::City& object, bool selected ) const
{
    UNREFERENCED_PARAMETER( selected );
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( const urban::District& object, bool selected ) const
{
    UNREFERENCED_PARAMETER( selected );
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( const urban::ResourceNetwork& object, bool selected ) const
{
    UNREFERENCED_PARAMETER( selected );
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( const urban::UndergroundNetwork& object, bool selected ) const
{
    UNREFERENCED_PARAMETER( selected );
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanDrawer::Draw
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanDrawer::Draw( Location_ABC& location ) const
{
    //TODO
}
