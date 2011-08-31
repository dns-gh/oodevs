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
    QSpinBox* spinBox = new QSpinBox( minValue_, maxValue_, step_, table()->viewport(), "spintableitem" );
    QObject::connect( spinBox, SIGNAL( valueChanged( int ) ), table(), SLOT( doValueChanged() ) );

    if( !text().isNull() )
        spinBox->setValue( text().toInt() );
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
        setText( static_cast< QSpinBox* >( widget )->text() );
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
    QDoubleSpinBox* spinBox = new QDoubleSpinBox( table()->viewport() );
    spinBox->setRange( minValue_, maxValue_ );
    spinBox->setSingleStep( step_ );
    spinBox->setDecimals( 2 );
    QObject::connect( spinBox, SIGNAL( valueChanged( double ) ), table(), SLOT( doValueChanged() ) );

    if( !text().isNull() )
        spinBox->setValue( text().toDouble() );
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
        setText( static_cast< QDoubleSpinBox* >( widget )->text() );
    else
        Q3TableItem::setContentFromEditor( widget );
}
