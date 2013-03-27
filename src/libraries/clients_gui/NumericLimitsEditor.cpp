// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "NumericLimitsEditor.h"
#include "moc_NumericLimitsEditor.cpp"
#include "clients_kernel/Tools.h"

using namespace gui;

// =============================================================================
// NumericLimitsEditor_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor_ABC constructor
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
NumericLimitsEditor_ABC::NumericLimitsEditor_ABC( QWidget* parent )
    : QWidget( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor_ABC destructor
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
NumericLimitsEditor_ABC::~NumericLimitsEditor_ABC()
{
    // NOTHING
}

// =============================================================================
// NumericLimitsEditor
// =============================================================================

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::CreateInterface
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template<>
void NumericLimitsEditor< int, RichSpinBox >::CreateInterface()
{
    minSpin_ = new RichSpinBox( "minSpin", 0, minValue_, maxValue_ );
    minSpin_->setValue( minValue_ );
    maxSpin_ = new RichSpinBox( "maxSpin", 0, minValue_, maxValue_ );
    maxSpin_->setValue( maxValue_ );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 2 );
    layout->addWidget( new QLabel( tools::translate( "NumericLimitsEditor", "Min" ) ) );
    layout->addWidget( minSpin_, 1 );
    layout->addWidget( new QLabel( tools::translate( "NumericLimitsEditor", "Max" ) ) );
    layout->addWidget( maxSpin_, 1 );
    connect( minSpin_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMinValueChanged( int ) ) );
    connect( maxSpin_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMaxValueChanged( int ) ) );
}

template<>
void NumericLimitsEditor< double, RichDoubleSpinBox >::CreateInterface()
{
    minSpin_ = new RichDoubleSpinBox( "minSpin", 0, minValue_, maxValue_ );
    minSpin_->setValue( minValue_ );
    maxSpin_ = new RichDoubleSpinBox( "maxSpin", 0, minValue_, maxValue_ );
    maxSpin_->setValue( maxValue_ );

    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 2 );
    layout->addWidget( new QLabel( tools::translate( "NumericLimitsEditor", "Min" ) ) );
    layout->addWidget( minSpin_, 1 );
    layout->addWidget( new QLabel( tools::translate( "NumericLimitsEditor", "Max" ) ) );
    layout->addWidget( maxSpin_, 1 );
    connect( minSpin_, SIGNAL( valueChanged( double ) ), this, SLOT( OnMinValueChanged( double ) ) );
    connect( maxSpin_, SIGNAL( valueChanged( double ) ), this, SLOT( OnMaxValueChanged( double ) ) );
}
