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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TooltipsLayer constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
TooltipsLayer::TooltipsLayer( kernel::GlTools_ABC& tools )
    : tools_( tools )
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
void TooltipsLayer::Paint( kernel::Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() )
        return;
    for( CIT_Tooltips it = tooltips_.begin(); it != tooltips_.end(); ++it )
        if( viewport.IsVisible( it->first ) )
            tools_.DrawImage( it->second, it->first );
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
