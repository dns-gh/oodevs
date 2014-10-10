// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorButton.h"
#include "moc_ColorButton.cpp"
#include "SignalAdapter.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::ColorButton( const QString& objectName, QWidget* parent /* = 0 */, QColor color /* = Qt::black */, bool hasAlpha /* = false */ )
    : RichWidget< QToolButton >( objectName, parent )
    , current_( color )
    , options_( hasAlpha ? QColorDialog::ShowAlphaChannel : static_cast< QColorDialog::ColorDialogOption >( 0 ) )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: ColorButton destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::~ColorButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Initialize
// Created: ABR 2014-10-02
// -----------------------------------------------------------------------------
void ColorButton::Initialize()
{
    setAutoRaise( true );
    setMinimumSize( 25, 25 );
    setMaximumWidth( height() * 4 / 3 );
    gui::connect( this, SIGNAL( clicked() ), [&](){
        QColor color = QColorDialog::getColor( current_, this, tr( "Select color" ), options_ );
        if( color.isValid() )
            SetColor( color );
    } );
    repaint();
}

// -----------------------------------------------------------------------------
// Name: ColorButton::drawButton
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::drawButton( QPainter* )
{
    paintEvent();
}

// -----------------------------------------------------------------------------
// Name: ColorButton::paintEvent
// Created: FPT 2011-05-03
// -----------------------------------------------------------------------------
void ColorButton::paintEvent( QPaintEvent* )
{
    QPainter paint;
    if ( paint.begin( this ) )
    {
        paint.fillRect( 4, 4, width() - 8, height() - 8, current_ );
        paint.drawRect( 4, 4, width() - 8, height() - 8 );
        paint.end();
    }
}

// -----------------------------------------------------------------------------
// Name: ColorButton::SetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::SetColor( const QColor& rgb )
{
    if( current_ == rgb )
        return;
    current_ = rgb;
    repaint();
    emit ColorChanged( current_ );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::GetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
QColor ColorButton::GetColor() const
{
    if( current_ == Qt::black )
        return QColor( 1, 1, 1 );
    return current_;
}
