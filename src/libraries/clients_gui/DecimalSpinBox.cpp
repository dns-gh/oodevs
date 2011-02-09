// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DecimalSpinBox.h"
#include "moc_DecimalSpinBox.cpp"

namespace
{
    float Round( float value ) 
    {
        return ( value > 0.0f ) ? floor( value + 0.5f ) : ceil( value - 0.5f );
    }
}

namespace gui
{

// -----------------------------------------------------------------------------
// Name: DecimalSpinBox constructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
DecimalSpinBox::DecimalSpinBox( QWidget* parent, float value /*= 0.f*/, unsigned short precision /*= 2*/, float min /*= 0*/, float max /*= 10*/, float gap /*= 0.1*/ )
    : QSpinBox( parent )
    , precision_( std::pow( 10.f, precision ) )
{
    if( value < min || value > max )
        throw std::runtime_error( __FUNCTION__ ": value not between min and max limit." );
    setMinValue( static_cast< int >( Round( min * precision_ ) ) );
    setMaxValue( static_cast< int >( Round( max * precision_ ) ) );
    setLineStep( static_cast< int >( Round( gap * precision_ ) ) );
    QSpinBox::setValue( static_cast< int >( Round( value * precision_ ) ) );
    setValidator( new QDoubleValidator( min, max, 2, parent ) );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBox destructor
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
DecimalSpinBox::~DecimalSpinBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBox::setValue
// Created: ABR 2011-01-31
// -----------------------------------------------------------------------------
void DecimalSpinBox::setValue( float value )
{
    QSpinBox::setValue( static_cast< int >( value * precision_ ) );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBox::mapValueToText
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
QString DecimalSpinBox::mapValueToText( int value )
{
    return QString( "%1" ).arg( static_cast< float >( value ) / precision_ );
}

// -----------------------------------------------------------------------------
// Name: DecimalSpinBox::mapTextToValue
// Created: ABR 2011-01-28
// -----------------------------------------------------------------------------
int DecimalSpinBox::mapTextToValue( bool*  )
{
    return static_cast< int >( precision_ * text().toFloat() );
}

} // namespace gui