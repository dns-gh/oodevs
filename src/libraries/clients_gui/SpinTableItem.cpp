// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SpinTableItem.h"
#include "RichSpinBox.h"

using namespace gui;

// =============================================================================
// Int
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SpinTableItem::createEditor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template<>
QWidget* SpinTableItem< int >::createEditor() const
{
    QSpinBox* spinBox = new RichSpinBox( table()->viewport(), minValue_, maxValue_, step_ );
    QObject::connect( spinBox, SIGNAL( valueChanged( int ) ), table(), SLOT( doValueChanged() ) );

    if( !text().isNull() )
        spinBox->setValue( locale_.toInt( text() ) );
    else
        spinBox->setValue( 0 );
    return spinBox;
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem::setContentFromEditor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template<>
void SpinTableItem< int >::setContentFromEditor( QWidget* widget )
{
    if( widget->inherits( "QSpinBox" ) )
        setText( locale_.toString( static_cast< QSpinBox* >( widget )->value() ) );
    else
        Q3TableItem::setContentFromEditor( widget );
}

// =============================================================================
// Double
// =============================================================================

// -----------------------------------------------------------------------------
// Name: SpinTableItem::createEditor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template<>
QWidget* SpinTableItem< double >::createEditor() const
{
    QDoubleSpinBox* spinBox = new RichDoubleSpinBox( table()->viewport(), minValue_, maxValue_, step_, 2 );
    QObject::connect( spinBox, SIGNAL( valueChanged( double ) ), table(), SLOT( doValueChanged() ) );

    if( !text().isNull() )
        spinBox->setValue( locale_.toDouble( text() ) );
    else
        spinBox->setValue( 0.0 );
    return spinBox;
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem::setContentFromEditor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template<>
void SpinTableItem< double >::setContentFromEditor( QWidget* widget )
{
    if( widget->inherits( "QDoubleSpinBox" ) )
        setText( locale_.toString( static_cast< QDoubleSpinBox* >( widget )->value() ) );
    else
        Q3TableItem::setContentFromEditor( widget );
}
