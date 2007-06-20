// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlTooltip.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Styles.h"
#include <qpainter.h>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GlTooltip constructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
GlTooltip::GlTooltip( const kernel::GlTools_ABC& tools )
    : tools_( tools )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTooltip destructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
GlTooltip::~GlTooltip()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const QColor& value )
{
    color_ = value;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void GlTooltip::Call( const Styles::Style& value )
{
    font_.setBold( &value == &Styles::bold );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_OperationalStatus& value )
{
    font_.setItalic( true );
    if ( value == eOperationalStatus_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eOperationalStatus_DetruitTotalement )
        color_.setRgb( 255, 0, 0 );
    Formatter< E_OperationalStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_ForceRatioStatus& value )
{
    font_.setBold( true );
    if ( value == eForceRatioStatus_Defavorable )
        color_.setRgb( 160, 0, 0 );
    else
        color_.setRgb( 0, 160, 0 );
    Formatter< E_ForceRatioStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_MeetingEngagementStatus& value )
{
    if( value != eMeetingEngagementStatus_None )
        Formatter< E_MeetingEngagementStatus >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::SubItem
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
Displayer_ABC& GlTooltip::SubItem( const QString& name )
{
    currentItem_ = name;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::StartDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::StartDisplay()
{
    color_ = Qt::black;
    font_  = QFont();
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::DisplayFormatted
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::EndDisplay
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::EndDisplay()
{
    if( ! currentItem_.isEmpty() )
        new_.push_back( T_Message( currentItem_ + " " + message_, T_Style( color_, font_ ) ) );
    else if( ! message_.isEmpty() )
        new_.push_back( T_Message( message_, T_Style( color_, font_ ) ) );
    DirtyImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::DirtyImage
// Created: AGE 2007-06-20
// -----------------------------------------------------------------------------
void GlTooltip::DirtyImage()
{
    if( current_.size() < new_.size()
     || new_.back() != current_.at( new_.size() - 1 ) )
        image_ = QImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Draw
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Draw( const geometry::Point2f& position )
{
    if( image_.isNull() )
        GenerateImage();
    new_.clear();
    tools_.DrawImage( image_, position );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::GenerateImage
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::GenerateImage()
{
    if( new_.empty() )
        return;
    QPainter p;
    QFontMetrics metrics( p.font() );
    int fontHeight = metrics.height();
    
    QPixmap pixmap( CreatePixmap() );
    pixmap.fill( Qt::magenta );
    p.begin( &pixmap );
        p.setPen( Qt::black );
        p.drawRect( pixmap.rect() );
        int y = fontHeight;
        for( CIT_Messages it = new_.begin(); it != new_.end(); ++it )
        {
            p.setPen( it->second.first );
            if( p.font() != it->second.second )
            {
                p.setFont( it->second.second );
                fontHeight = QFontMetrics( p.font() ).height();
            }
            p.drawText( 4, y, it->first );
            
            y += fontHeight;
        }
    p.end();
    image_ = pixmap.convertToImage().mirror();
    RestoreAlpha();
    std::swap( new_, current_ );
    new_.clear();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::RestoreAlpha
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::RestoreAlpha()
{
    if( image_.isNull() )
        return;
    unsigned int* data = (unsigned int*)image_.bits();
    for( int i = image_.width() * image_.height(); i > 0; --i )
    {
        if( *data == 0x00FF00FF ) 
            *data = 0xE0FFFFFF;
        else
            *data |= 0xFF000000;
        ++data;
    }
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::CreatePixmap
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
QPixmap GlTooltip::CreatePixmap()
{
    int w = 0;
    int h = 0;
    for( CIT_Messages it = new_.begin(); it != new_.end(); ++it )
    {
        const QFontMetrics metrics( it->second.second );
        const QRect bounds = metrics.boundingRect( it->first );
        w = std::max( w, bounds.width() );
        h += bounds.height() + 2;
    }
    return QPixmap( w + 8, h + 4, 32 );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Hide
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Hide()
{
    current_.clear();
    new_.clear();
    image_ = QImage();
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::operator kernel::Displayer_ABC&
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
GlTooltip::operator kernel::Displayer_ABC&()
{
    return *this;
}
