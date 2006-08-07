// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "GlTooltip.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: GlTooltip constructor
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
GlTooltip::GlTooltip()
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
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_EtatOperationnel& value )
{
    if ( value == eEtatOperationnel_DetruitTactiquement )
        color_.setRgb( 255, 128, 0 );
    else if( value == eEtatOperationnel_DetruitTotalement )
        color_.setRgb( 255, 0, 0 );
    Formatter< E_EtatOperationnel >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_EtatRapFor& value )
{
    if ( value == eEtatRapFor_Defavorable )
        color_.setRgb( 255, 154, 154 );
    else
        color_.setRgb( 204, 255, 204 );
    Formatter< E_EtatRapFor >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Call
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Call( const E_EtatCombatRencontre& value )
{
    if( value != eEtatCombatRencontre_None )
        Formatter< E_EtatCombatRencontre >()( value, *this );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::SubItem
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
Displayer_ABC& GlTooltip::SubItem( const char* name )
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
    color_ = Qt::white;
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
        messages_.push_back( T_Message( currentItem_ + " : " + message_, color_ ) );
    else if( ! message_.isEmpty() )
        messages_.push_back( T_Message( message_, color_ ) );
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
    if( image_.bits() )
    {
        glRasterPos2fv( (float*)( &position ) );
        glBitmap(0, 0, 0, 0, 0, - image_.height(), 0 );
        glDrawPixels( image_.width(), image_.height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, image_.bits() );
    }
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::GenerateImage
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::GenerateImage()
{
    if( messages_.empty() )
        return;
    QPainter p;
    const QFontMetrics metrics( p.font() );
    const int fontHeight = metrics.height();
    
    QPixmap pixmap( CreatePixmap( p ) );
    pixmap.fill( Qt::magenta );
    p.begin( &pixmap );
        int y = fontHeight;
        for( CIT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        {
            p.setPen( it->second );
            p.drawText( 4, y, it->first );
            y += fontHeight;
        }
    p.end();
    image_ = pixmap.convertToImage().mirror();
    RestoreAlpha();

    messages_.clear();
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
            *data = 0x80000000;
        else
            *data |= 0xFF000000;
        ++data;
    }
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::CreatePixmap
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
QPixmap GlTooltip::CreatePixmap( QPainter& p )
{
    const QFontMetrics metrics( p.font() );
    int w = 0;
    int h = 0;
    for( CIT_Messages it = messages_.begin(); it != messages_.end(); ++it )
    {
        const QRect bounds = metrics.boundingRect( it->first );
        w = std::max( w, bounds.width() );
        h += bounds.height() + 2;
    }
    return QPixmap( w + 8, h, 32 );
}

// -----------------------------------------------------------------------------
// Name: GlTooltip::Hide
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void GlTooltip::Hide()
{
    messages_.clear();
    image_ = QImage();
}

