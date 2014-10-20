// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TooltipsLayer.h"

#include "GLView_ABC.h"
#include "Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TooltipsLayer constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
TooltipsLayer::TooltipsLayer( kernel::Controllers& controllers, GLView_ABC& tools )
    : Layer( controllers, tools, eLayerTypes_Tooltips )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TooltipsLayer destructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
TooltipsLayer::~TooltipsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TooltipsLayer::Paint
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void TooltipsLayer::Paint( Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() )
        return;
    for( auto it = tooltips_.begin(); it != tooltips_.end(); ++it )
        if( viewport.IsVisible( it->first ) )
            view_.DrawImage( it->second, it->first );
    tooltips_.clear();
}

// -----------------------------------------------------------------------------
// Name: TooltipsLayer::Draw
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void TooltipsLayer::Draw( const geometry::Point2f& position, const QImage& image )
{
    tooltips_.push_back( std::make_pair( position, image ) );
}
