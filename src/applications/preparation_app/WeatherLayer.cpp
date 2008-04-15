// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "WeatherLayer.h"
#include "preparation/LocalWeather.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_gui/ExclusiveEventStrategy.h"

// -----------------------------------------------------------------------------
// Name: WeatherLayer constructor
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
WeatherLayer::WeatherLayer( kernel::GlTools_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy )
    : tools_( tools )
    , eventStrategy_( eventStrategy )
    , firstPointSet_( false )
    , isEditing_( false )
    , currentWeather_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer destructor
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
WeatherLayer::~WeatherLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::Paint
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    const geometry::Point2f topRight( bottomRight_.X(), topLeft_.Y() );
    const geometry::Point2f bottomLeft( topLeft_.X(), bottomRight_.Y() );

    // $$$$ SBO 2006-12-21: viewport
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glColor4f( 1, 0, 0, 0.5f );
    glLineWidth( 5.f );
        tools_.DrawLine( topLeft_, topRight );
        tools_.DrawLine( topRight, bottomRight_ );
        tools_.DrawLine( bottomRight_, bottomLeft );
        tools_.DrawLine( bottomLeft, topLeft_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::HandleMousePress
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
bool WeatherLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->state() == Qt::LeftButton && isEditing_ )
    {
        currentWeather_->SetPosition( topLeft_, point );
        eventStrategy_.ReleaseExclusiveFocus();
        isEditing_ = false;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::HandleMouseMove
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
bool WeatherLayer::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->state() == Qt::LeftButton && isEditing_ )
    {
        if( !firstPointSet_ )
        {
            topLeft_ = point;
            firstPointSet_ = true;
        }
        else
            bottomRight_ = point;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::SetPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherLayer::SetPosition( const LocalWeather& weather )
{
    topLeft_ = weather.GetTopLeft();
    bottomRight_ = weather.GetBottomRight();
}

// -----------------------------------------------------------------------------
// Name: WeatherLayer::EditPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherLayer::StartEdition( LocalWeather& weather )
{
    currentWeather_ = &weather;
    SetPosition( weather );
    eventStrategy_.TakeExclusiveFocus( *this );
    isEditing_ = true;
    firstPointSet_ = false;
}
