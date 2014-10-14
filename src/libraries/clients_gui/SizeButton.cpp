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
#include "SignalAdapter.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SizeButton constructor
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
SizeButton::SizeButton( kernel::OptionsController& options,
                        const QString& objectName,
                        const std::string& optionName,
                        float min,
                        float max,
                        float value,
                        QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QSlider > >( options, objectName, optionName, parent )
{
    setTickPosition( QSlider::Below );
    setRange( static_cast< int >( 2 * min ), static_cast< int >( 2 * max ) );
    setTickInterval( 2 );
    setPageStep( 1 );
    setOrientation( Qt::Horizontal );
    setFixedWidth( 130 );
    gui::connect( this, SIGNAL( valueChanged( int ) ), [=,&options](){
        options.Change( optionName, GetSize() );
    } );
    setValue( static_cast< int >( 2 * value ) );
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
// Name: SizeButton::SetSize
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void SizeButton::SetSize( float value )
{
    setValue( static_cast< int >( value * 2.f ) );
}

// -----------------------------------------------------------------------------
// Name: SizeButton::GetSize
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
float SizeButton::GetSize() const
{
    return value() * 0.5f;
}
