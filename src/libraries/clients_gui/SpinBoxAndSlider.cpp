// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SpinBoxAndSlider.h"
#include "moc_SpinBoxAndSlider.cpp"
#include "RichSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider constructor
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
SpinBoxAndSlider::SpinBoxAndSlider( QWidget* parent, int& value, int min /* = 0 */, int max /* = 10 */, int gap /* = 1 */,
                                    Qt::Orientation orientation /* = Qt::Horizontal */, Qt::Orientation sliderOrientation /* = Qt::Horizontal */,
                                    bool spinboxFirst /* = true */ )
    : QWidget( parent )
    , value_    ( value )
{
    if( orientation == Qt::Horizontal )
        layout_ = new Q3HBoxLayout( this );
    else
        layout_ = new Q3VBoxLayout( this );
    spinbox_ = new RichSpinBox( "default", this, min, max, gap );
    spinbox_->setValue( value_ );
    spinbox_->setMinimumWidth( 60 );
    slider_ = new QSlider( min, max, gap, value_, sliderOrientation, this );    if( spinboxFirst )
    {
        layout_->addWidget( spinbox_ );
        layout_->addWidget( slider_ );
    }
    else
    {
        layout_->addWidget( slider_ );
        layout_->addWidget( spinbox_ );
    }
    connect( spinbox_, SIGNAL( valueChanged( int ) ), slider_, SLOT( setValue( int ) ) );
    connect( slider_,  SIGNAL( valueChanged( int ) ), spinbox_, SLOT( setValue( int ) ) );
    connect( slider_,  SIGNAL( valueChanged( int ) ), SLOT( setValue( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider destructor
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
SpinBoxAndSlider::~SpinBoxAndSlider()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider::setMargin
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void SpinBoxAndSlider::setMargin( int value )
{
    layout_->setMargin( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider::setSpacing
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void SpinBoxAndSlider::setSpacing( int value )
{
    layout_->setSpacing( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider::setEnabled
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void SpinBoxAndSlider::setEnabled( bool value )
{
    spinbox_->setEnabled( value );
    slider_->setEnabled( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider::value
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
int SpinBoxAndSlider::value() const
{
    assert( slider_->value() == spinbox_->value() );
    return slider_->value();
}

// -----------------------------------------------------------------------------
// Name: SpinBoxAndSlider::setValue
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void SpinBoxAndSlider::setValue( int value )
{
    value_ = value;
    if( slider_->value() != value )
        slider_->setValue( value );
}
