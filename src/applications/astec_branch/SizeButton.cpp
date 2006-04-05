// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SizeButton.h"
#include "moc_SizeButton.cpp"

#include <qpopupmenu.h>
#include <qslider.h>

// -----------------------------------------------------------------------------
// Name: SizeButton constructor
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
SizeButton::SizeButton( QWidget* parent /*= 0*/, const char* name /*= 0*/ )
    : QPushButton( parent, name )
    , slider_( 0 )
    , size_( 0 )
    , prefix_()
    , valueLabel_( false )
{
    setText( name );
    QPopupMenu* menu = new QPopupMenu( this );
    slider_ = new QSlider( 0, 100, 1, 0, Qt::Orientation::Horizontal, menu );
    slider_->setTickmarks( QSlider::Below );
    slider_->setTickInterval( 10 );
    menu->insertItem( slider_ );
    setPopup( menu );
    connect( slider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: SizeButton destructor
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
SizeButton::~SizeButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SizeButton::OnValueChanged
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
void SizeButton::OnValueChanged( int value )
{
    size_ = value;
    if( valueLabel_ )
        setText( QString( name() ).append( QString::number( size_ ) ).append( prefix_.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: SizeButton::EnableValueLabel
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
void SizeButton::EnableValueLabel( const std::string& prefix /*= ""*/ )
{
    if( !prefix.empty() )
        prefix_ = prefix;
    valueLabel_ = true;
}
