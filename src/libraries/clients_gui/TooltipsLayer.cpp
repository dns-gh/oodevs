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
#include "Tooltip.h"
#include "GLView_ABC.h"
#include "Viewport_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TooltipsLayer constructor
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
TooltipsLayer::TooltipsLayer( kernel::Controllers& controllers, GLView_ABC& tools )
    : Layer( controllers, tools, eLayerTypes_Tooltips )
    , alpha_( 1 )
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

namespace
{
    QImage ApplyAlpha( const QImage& image, float alpha )
    {
        if( image.isNull() )
            return image;
        QImage result( image.width(), image.height(), QImage::Format_ARGB32_Premultiplied );
        result.fill( Qt::transparent );
        QPainter painter( &result );
        painter.setOpacity( alpha );
        painter.drawImage( image.rect(), image );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: TooltipsLayer::Paint
// Created: SBO 2008-04-11
// -----------------------------------------------------------------------------
void TooltipsLayer::Paint( Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() )
        return;
    if( alpha_ != GetAlpha() )
    {
        alpha_ = GetAlpha();
        cache_.clear();
    }
    for( auto it = tooltips_.begin(); it != tooltips_.end(); ++it )
        if( viewport.IsVisible( it->first ) )
        {
            auto& image = cache_[ it->second ];
            if( image.isNull() )
                image = ApplyAlpha( it->second.GenerateImage().mirror(), GetAlpha() );
            view_.DrawImage( image, it->first );
        }
    tooltips_.clear();
}

void TooltipsLayer::Draw( const geometry::Point2f& position, const Tooltip& tooltip )
{
    if( !tooltip.Empty() )
        tooltips_.insert( std::make_pair( position, tooltip ) );
}
