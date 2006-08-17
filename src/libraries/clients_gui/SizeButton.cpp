// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SizeButton.h"
#include "moc_SizeButton.cpp"

#include <qpopupmenu.h>
#include <qslider.h>

// -----------------------------------------------------------------------------
// Name: SizeButton constructor
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
SizeButton::SizeButton( QWidget* parent /*= 0*/, const char* name /*= 0*/, float value /*= 1*/ )
    : QPushButton( parent, name )
    , slider_( 0 )
    , size_( value )
    , prefix_()
    , valueLabel_( false )
    , changed_( false )
    , previous_( value )
{
    setText( name );
    QPopupMenu* menu = new QPopupMenu( this );
    slider_ = new QSlider( 0, 20, 1, 0, Qt::Orientation::Horizontal, menu );
    slider_->setTickmarks( QSlider::Below );
    slider_->setTickInterval( 2 );
    
    menu->insertItem( slider_ );
    setPopup( menu );
    connect( slider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );

    slider_->setValue( int( 2 * value ) );
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
    if( ! changed_ )
        previous_ = size_;
    size_ = value * 0.5;
    changed_ = true;
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
    OnValueChanged( slider_->value() );
}

// -----------------------------------------------------------------------------
// Name: SizeButton::GetSize
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
float SizeButton::GetSize() const
{
    return size_;
}

// -----------------------------------------------------------------------------
// Name: SizeButton::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void SizeButton::Revert()
{
    changed_ = false;
    size_ = previous_;
    if( valueLabel_ )
        setText( QString( name() ).append( QString::number( size_ ) ).append( prefix_.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: SizeButton::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void SizeButton::Commit()
{
    changed_ = false;
    previous_ = size_;
}
