// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DecimalSpinBoxAndSlider.h"
#include "moc_DecimalSpinBoxAndSlider.cpp"
#include "RichSpinBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
DecimalSpinBoxAndSlider::DecimalSpinBoxAndSlider( QWidget* parent, float& value, unsigned short precision /* = 2*/, float min /* = 0*/, float max /* = 10*/, float gap /* = 0.1*/,
                                                  Qt::Orientation orientation /* = Qt::Horizontal*/, Qt::Orientation sliderOrientation /* = Qt::Horizontal*/, bool spinboxFirst /* = true*/ )
    : QWidget( parent )
    , precision_( std::pow( 10.f, precision ) )
    , value_    ( value )
{
    if( orientation == Qt::Horizontal )
        layout_ = new Q3HBoxLayout( this );
    else
        layout_ = new Q3VBoxLayout( this );
    spinbox_ = new RichDoubleSpinBox( "decimal", this, min, max, gap, precision );
    spinbox_->setValue( value_ );
    spinbox_->setMinimumWidth( 30 * precision );
    slider_ = new QSlider( static_cast< int >( precision_ * min ), static_cast< int >( precision_ * max ), static_cast< int >( precision_ * gap ), static_cast< int >( precision_ * value_ ), sliderOrientation, this );
    if( spinboxFirst )
    {
        layout_->addWidget( spinbox_ );
        layout_->addWidget( slider_ );
    }
    else
    {
        layout_->addWidget( slider_ );
        layout_->addWidget( spinbox_ );
    }
    connect( spinbox_, SIGNAL( valueChanged( double ) ), SLOT( setSliderValue( double ) ) );
    connect( slider_,  SIGNAL( valueChanged( int ) ), SLOT( setSpinboxValue( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider destructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
DecimalSpinBoxAndSlider::~DecimalSpinBoxAndSlider()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setMargin
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setMargin( int value )
{
    layout_->setMargin( value );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setSpacing
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setSpacing( int value )
{
    layout_->setSpacing( value );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::value
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
float DecimalSpinBoxAndSlider::value() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setSpinboxValue
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setSpinboxValue( int value )
{
    value_ = value / precision_;
    bool wasBlocked = spinbox_->blockSignals( true );
    spinbox_->setValue( value_ );
    spinbox_->blockSignals( wasBlocked );
    emit valueChanged( value_ );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setSliderValue
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setSliderValue( double value )
{
    value_ = static_cast< float >( value );
    bool wasBlocked = slider_->blockSignals( true );
    slider_->setValue( static_cast< int >( value * precision_ ) );
    slider_->blockSignals( wasBlocked );
    emit valueChanged( value_ );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setValue
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setValue( float value )
{
    value_ = value;
    spinbox_->setValue( value );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBoxAndSlider::setEnabled
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void DecimalSpinBoxAndSlider::setEnabled( bool value )
{
    spinbox_->setEnabled( value );
    slider_->setEnabled( value );
}
