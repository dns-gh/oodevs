// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OptionWidgets.h"
#include "SignalAdapter.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

OptionColorButton::OptionColorButton( kernel::OptionsController& options,
                                      const QString& objectName,
                                      const std::string& optionName,
                                      const QColor& color /* = Qt::black */,
                                      QWidget* parent /* = 0 */ )
    : OptionWidget< ColorButton >( options, objectName, optionName, parent )
{
    SetColor( color );
    gui::connect( this, SIGNAL( ColorChanged( const QColor& ) ), [=,&options]{
        options.Change( optionName, GetColor().name() );
    } );
}

OptionColorButton::~OptionColorButton()
{
    // NOTHING
}

void OptionColorButton::OnOptionChanged( const kernel::OptionVariant& value )
{
    SetColor( QColor( value.To< QString >() ) );
}

// -----------------------------------------------------------------------------

OptionCheckBox::OptionCheckBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& text,
                                QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QCheckBox > >( options, objectName, optionName, parent )
{
    setText( text );
    gui::connect( this, SIGNAL( stateChanged( int ) ), [=,&options]{
        options.Change( optionName, isChecked() );
    } );
}

OptionCheckBox::~OptionCheckBox()
{
    // NOTHING
}

void OptionCheckBox::OnOptionChanged( const kernel::OptionVariant& value )
{
    setChecked( value.To< bool >() );
}

// -----------------------------------------------------------------------------

OptionSpinBox::OptionSpinBox( kernel::OptionsController& options,
                                   const QString& objectName,
                                   const std::string& optionName,
                                   int min,
                                   int max,
                                   QWidget* parent /* = 0 */ )
    : OptionWidget< RichSpinBox >( options, objectName, optionName, parent )
{
    setRange( min, max );
    gui::connect( this, SIGNAL( valueChanged( int ) ), [=,&options]{
        options.Change( optionName, value() );
    } );
}

OptionSpinBox::~OptionSpinBox()
{
    // NOTHING
}

void OptionSpinBox::OnOptionChanged( const kernel::OptionVariant& value )
{
    setValue( value.To< int >() );
}

// -----------------------------------------------------------------------------

OptionSlider::OptionSlider( kernel::OptionsController& options,
                            const QString& objectName,
                            const std::string& optionName,
                            int min,
                            int max,
                            QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QSlider > >( options, objectName, optionName, parent )
{
    setOrientation( Qt::Horizontal );
    setRange( min, max );
    gui::connect( this, SIGNAL( valueChanged( int ) ), [=,&options]{
        options.Change( optionName, value() );
    } );
}

OptionSlider::~OptionSlider()
{
    // NOTHING
}

void OptionSlider::OnOptionChanged( const kernel::OptionVariant& value )
{
    setValue( value.To< int >() );
}

// -----------------------------------------------------------------------------

OptionGroupBox::OptionGroupBox( kernel::OptionsController& options,
                                const QString& objectName,
                                const std::string& optionName,
                                const QString& title,
                                QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QGroupBox > >( options, objectName, optionName, parent )
{
    setTitle( title );
    setCheckable( true );
    gui::connect( this, SIGNAL( clicked( bool ) ), [=,&options]{
        options.Change( optionName, isChecked() );
    } );
}

OptionGroupBox::~OptionGroupBox()
{
    // NOTHING
}

void OptionGroupBox::OnOptionChanged( const kernel::OptionVariant& value )
{
    setChecked( value.To< bool >() );
}

// -----------------------------------------------------------------------------

OptionDial::OptionDial( kernel::OptionsController& options,
                        const QString& objectName,
                        const std::string& optionName,
                        int min,
                        int max,
                        QWidget* parent /* = 0 */ )
    : OptionWidget< RichWidget< QDial > >( options, objectName, optionName, parent )
{
    setRange( min, max );
    gui::connect( this, SIGNAL( valueChanged( int ) ), [=,&options]{
        options.Change( optionName, value() );
    } );
}

OptionDial::~OptionDial()
{
    // NOTHING
}

void OptionDial::OnOptionChanged( const kernel::OptionVariant& value )
{
    setValue( value.To< int >() );
}
