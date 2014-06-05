// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "GlButton.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/GlTooltip.h"

#pragma warning( push , 0 )
#include <boost/bind.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: GlButton constructor
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
GlButton::GlButton( const QString& label, const gui::GlTools_ABC& tools )
    : tools_    ( tools )
    , label_    ( label )
    , current_  ( 0 )
    , alignment_( Qt::AlignLeft | Qt::AlignTop )
    , xMargin_  ( 0 )
    , yMargin_  ( 0 )
{
    QFont font( "Arial", 12, QFont::Bold );
    QFontMetrics metrics( font );
    width_  = metrics.width( label ) + 8;
    height_ = metrics.height() + 4;

    default_ = CreateTooltip( font, false );
    hover_   = CreateTooltip( font, true );
    current_ = default_.get();
}

// -----------------------------------------------------------------------------
// Name: GlButton destructor
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
GlButton::~GlButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlButton::CreateTooltip
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
std::unique_ptr< gui::GlTooltip > GlButton::CreateTooltip( const QFont& font, bool hover )
{
    std::unique_ptr< gui::GlTooltip_ABC > tooltip = tools_.CreateTooltip();
    std::unique_ptr< gui::GlTooltip > concrete( (gui::GlTooltip*)tooltip.get() );
    tooltip.release();
    concrete->SetFrameDrawer( boost::bind( &GlButton::DrawFrame, this, _1, _2, hover ) );
    kernel::Displayer_ABC& display = *concrete;
    display.Start( font )
                .Add( label_ )
           .End();
    return concrete;
}

// -----------------------------------------------------------------------------
// Name: GlButton::Draw
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlButton::Draw( const geometry::Rectangle2f& box )
{
    if( !current_ )
        return;
    float x = 0, y = 0;
    if( alignment_ & Qt::AlignRight )
        x = box.Right() - tools_.Pixels() * ( width_  + xMargin_ );
    else
        x = box.Left() + tools_.Pixels() * xMargin_;
    if( alignment_ & Qt::AlignTop )
        y = box.Top() - tools_.Pixels() * ( height_ + yMargin_ );
    else if( alignment_ & Qt::AlignBottom )
        y = box.Bottom() + tools_.Pixels() * yMargin_;
    else
        y = box.Bottom() + ( box.Height() - height_ * tools_.Pixels() ) * 0.5f;
    position_.Set( x, y );
    current_->Draw( position_, width_, height_, tools_.Pixels() );
}

// -----------------------------------------------------------------------------
// Name: GlButton::SetPosition
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlButton::SetPosition( int alignment )
{
    alignment_ = alignment;
}

// -----------------------------------------------------------------------------
// Name: GlButton::SetMargin
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlButton::SetMargin( int x, int y )
{
    xMargin_ = x;
    yMargin_ = y;
}

// -----------------------------------------------------------------------------
// Name: GlButton::HandleMousePress
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
bool GlButton::HandleMousePress( const geometry::Point2f& point )
{
    return HandleMouseMove( point );
}

// -----------------------------------------------------------------------------
// Name: GlButton::HandleMouseMove
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
bool GlButton::HandleMouseMove( const geometry::Point2f& point )
{
    const float x = point.X() - position_.X();
    const float y = point.Y() - position_.Y();
    const bool hover = x > 0 && x < width_ * tools_.Pixels() && y > 0 && y < height_ * tools_.Pixels();
    current_ = hover ? hover_.get() : default_.get();
    return hover;
}

// -----------------------------------------------------------------------------
// Name: GlButton::DrawFrame
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void GlButton::DrawFrame( QPainter& painter, const QRect& rect, bool /*hover*/ )
{
    QStyleOption* opt = new QStyleOption();
    QStyle* style = QApplication::style();
    QPushButton button( label_, 0 );
    painter.fillRect( rect, QColor( 159, 174, 216 ) );
    opt->rect = rect;
    style->drawControl( QStyle::CE_PushButton, opt, &painter);
}

// -----------------------------------------------------------------------------
// Name: GlButton::Label
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
QString GlButton::Label() const
{
    return label_;
}

// -----------------------------------------------------------------------------
// Name: GlButton::BoundingBox
// Created: SBO 2009-02-11
// -----------------------------------------------------------------------------
geometry::Rectangle2f GlButton::BoundingBox() const
{
    return geometry::Rectangle2f( position_.X(), position_.Y() + height_, position_.X() + width_, position_.Y() );
}
