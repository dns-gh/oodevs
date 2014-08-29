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
#include "ObjectNameManager.h"

using namespace gui;

namespace
{
    unsigned int Convert( const std::string& color )
    {
        unsigned int alpha;
        std::stringstream ss( color );
        ss >> std::hex >> alpha;
        return alpha;
    }
}

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: LGY 2011-09-30
// -----------------------------------------------------------------------------
ColorButton::ColorButton( const QString& objectName, QWidget* parent, const std::string& color )
    : RichWidget< QToolButton >( objectName, parent )
    , options_( QColorDialog::ShowAlphaChannel )
{
    setAutoRaise( true );
    current_.setNamedColor( color.substr( 0, 7 ).c_str() );
    current_.setAlpha( Convert( color.substr( 7, 8 ) ) );
    previous_ = current_;
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
    repaint();
}

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::ColorButton( const QString& objectName, QWidget* parent /* = 0*/, const char* text /* = 0*/, QColor color /* = black*/ )
    : RichWidget< QToolButton >( objectName, parent )
    , previous_( color )
    , current_( color )
    , options_( static_cast< QColorDialog::ColorDialogOption >( 0 ) )
{
    setText( text );
    setAutoRaise( true );
    setMinimumSize( 25, 25 );
    setMaximumWidth( height() * 4 / 3 );
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
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
    if( paint.begin( this ) )
    {
        paint.fillRect( 4, 4, width() - 8, height() - 8, current_ );
        paint.drawRect( 4, 4, width() - 8, height() - 8 );
        paint.end();
    }
}

// -----------------------------------------------------------------------------
// Name: ColorButton::OnClick
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::OnClick()
{
    QColor color = QColorDialog::getColor( current_, this, tr( "Select color" ), options_ );
    if( color.isValid() )
        SetColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::SetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::SetColor( const QColor& rgb )
{
    if( current_ == rgb )
        return;
    previous_ = current_;
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
    return current_;
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Revert
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::Revert()
{
    current_ = previous_;
    repaint();
    emit ColorChanged( current_ );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ColorButton::Commit()
{
    previous_ = current_;
}
