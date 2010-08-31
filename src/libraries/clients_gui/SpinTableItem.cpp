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

// -----------------------------------------------------------------------------
// Name: SpinTableItem::SpinTableItem
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
SpinTableItem::SpinTableItem( QTable* table, int minValue, int maxValue, int step /*= 1*/ )
    : QTableItem( table, QTableItem::WhenCurrent )
    , minValue_( minValue )
    , maxValue_( maxValue )
    , step_( step )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem destructor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
SpinTableItem::~SpinTableItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem::createEditor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
QWidget* SpinTableItem::createEditor() const
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
void SpinTableItem::setContentFromEditor( QWidget* widget )
{
    if( widget->inherits( "QSpinBox" ) )
        setText( static_cast< QSpinBox* >( widget )->text() );
    else
        QTableItem::setContentFromEditor( widget );
}
