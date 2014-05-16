// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "StandardModel.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor constructor
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
NumericLimitsEditor< NumericType, SpinBox >::NumericLimitsEditor( QWidget* parent,
                                                                  const T_Extractor& extractor,
                                                                  NumericType min,
                                                                  NumericType max )
    : NumericLimitsEditor_ABC( parent )
    , extractor_( extractor )
    , minValue_( min != max ? min : std::numeric_limits< NumericType >::min() )
    , maxValue_( min != max ? max : std::numeric_limits< NumericType >::max() )
{
    CreateInterface();
    minSpin_->setRange( minValue_, maxValue_ );
    maxSpin_->setRange( minValue_, maxValue_ );
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor destructor
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
NumericLimitsEditor< NumericType, SpinBox >::~NumericLimitsEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::OnMinValueChanged
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
void NumericLimitsEditor< NumericType, SpinBox >::OnMinValueChanged( NumericType value )
{
    if( value > maxSpin_->value() )
        minSpin_->setValue( maxSpin_->value() );
    maxSpin_->setMinimum( minSpin_->value() );
    emit( ValueChanged() );
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::OnMaxValueChanged
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
void NumericLimitsEditor< NumericType, SpinBox >::OnMaxValueChanged( NumericType value )
{
    if( minSpin_->value() > value )
        maxSpin_->setValue( minSpin_->value() );
    minSpin_->setMaximum( maxSpin_->value() );
    emit( ValueChanged() );
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::Clear
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
void NumericLimitsEditor< NumericType, SpinBox >::Clear()
{
    minSpin_->setValue( minValue_ );
    maxSpin_->setValue( maxValue_ );
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::ApplyFilter
// Created: JSR 2012-09-17
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
bool NumericLimitsEditor< NumericType, SpinBox >::Apply( QStandardItem& item ) const
{
    bool valid = true;
    auto value = extractor_( item, valid );
    if( !valid )
        return false;
    return value >= minSpin_->value() && value <=  maxSpin_->value();
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::GetWidget
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
QWidget* NumericLimitsEditor< NumericType, SpinBox >::GetWidget()
{
    return this;
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::GetMenuContent
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
QWidget* NumericLimitsEditor< NumericType, SpinBox >::GetMenuContent()
{
    return this;
}

} //! namespace gui
