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
NumericLimitsEditor< NumericType, SpinBox >::NumericLimitsEditor( QWidget* parent /*= 0*/ )
    : NumericLimitsEditor_ABC( parent )
    , minValue_( std::numeric_limits< NumericType >::min() )
    , maxValue_( std::numeric_limits< NumericType >::max() )
{
    CreateInterface();
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
// Name: NumericLimitsEditor::SetExtractor
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
void NumericLimitsEditor< NumericType, SpinBox >::SetExtractor( T_Extractor extractor )
{
    extractor_ = extractor;
}

// -----------------------------------------------------------------------------
// Name: NumericLimitsEditor::SetMinMax
// Created: ABR 2012-06-20
// -----------------------------------------------------------------------------
template< typename NumericType, typename SpinBox >
void NumericLimitsEditor< NumericType, SpinBox >::SetMinMax( NumericType min, NumericType max )
{
    minValue_ = min;
    maxValue_ = max;
    minSpin_->setRange( minValue_, maxValue_ );
    maxSpin_->setRange( minValue_, maxValue_ );
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
bool NumericLimitsEditor< NumericType, SpinBox >::ApplyFilter( QStandardItem& item, StandardModel& model ) const
{
    const kernel::Entity_ABC* entity = model.GetDataFromItem< kernel::Entity_ABC >( item );
    if( entity )
    {
        bool valid = true;
        NumericType extractedValue = extractor_( *entity, valid );
        if( !valid )
            return false;
        return extractedValue >= minSpin_->value() && extractedValue <=  maxSpin_->value();
    }
    return false;
}


} //! namespace gui
