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
#include <math.h>

using namespace gui;
// -----------------------------------------------------------------------------
// Name: SizeButton constructor
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
SizeButton::SizeButton( QWidget* parent /* = 0*/, const char* name /* = 0*/, float value /* = 1*/ )
    : QSlider( parent, name )
    , label_( parent )
    , size_( value )
    , prefix_()
    , changed_( false )
    , previous_( value )
{
    setTickPosition( QSlider::Below );
    setRange( 0, 20 );
    setTickInterval( 2 );
    setPageStep( 1 );
    setOrientation( Qt::Horizontal );
    label_.setText( QString( name ).append( QString::number( 1 ) ).append( prefix_ ) );

    connect( this, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );
    setValue( int( 2 * value ) );
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
    size_ = value * 0.5f;
    changed_ = true;
    if( valueLabel_ )
    {
        if ( size_ == 10 )
            label_.setText( QString( name() ).append( " " ).append( QString::number( size_ ) ).append( prefix_ ) );
        else if ( size_ == floor( size_ ) )
            label_.setText( QString( name() ).append( "   " ).append( QString::number( size_ ) ).append( prefix_ ) );
        else
            label_.setText( QString( name() ).append( QString::number( size_ ) ).append( prefix_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SizeButton::EnableValueLabel
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
void SizeButton::EnableValueLabel( const QString& prefix /*= ""*/ )
{
    if( !prefix.isEmpty() )
        prefix_ = prefix;
    valueLabel_ = true;
    OnValueChanged( value() );
}

// -----------------------------------------------------------------------------
// Name: SizeButton::SetSize
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void SizeButton::SetSize( float value )
{
    setValue( int( value * 2.f ) );
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
    {
        if ( size_ == 10 )
            label_.setText( QString( name() ).append( " " ).append( QString::number( size_ ) ).append( prefix_ ) );
        else if ( size_ == floor( size_ ) )
            label_.setText( QString( name() ).append( "   " ).append( QString::number( size_ ) ).append( prefix_ ) );
        else
            label_.setText( QString( name() ).append( "" ).append( QString::number( size_ ) ).append( prefix_ ) );
    }
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
