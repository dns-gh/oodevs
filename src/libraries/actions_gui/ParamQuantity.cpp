// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamQuantity.h"
#include "moc_ParamQuantity.cpp"
#include "InterfaceBuilder_ABC.h"
#include "actions/Quantity.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/Units.h"
#include <QtGui/QSpinBox.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamQuantity constructor
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
ParamQuantity::ParamQuantity( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , modifier_( 0 )
    , min_( 0 )
    , max_( std::numeric_limits< int >::max() )
    , spinBox_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity destructor
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
ParamQuantity::~ParamQuantity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::BuildInterface
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
QWidget* ParamQuantity::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    spinBox_ = new QSpinBox( min_, max_, 1, parent );
    spinBox_->setValue( min_ );
    spinBox_->setSuffix( suffix_ );
    connect( spinBox_, SIGNAL( valueChanged( int ) ), SLOT( OnChanged( int ) ) );
    layout->addWidget( spinBox_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::InternalCheckValidity
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
bool ParamQuantity::InternalCheckValidity() const
{
    return modifier_ >= min_ && modifier_ <= max_;
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::CommitTo
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamQuantity::CommitTo( actions::ParameterContainer_ABC& parameter ) const
{
    if( IsChecked() && spinBox_->isEnabled() )
        parameter.AddParameter( *new actions::parameters::Quantity( parameter_, modifier_ ) );
    else
        parameter.AddParameter( *new actions::parameters::Quantity( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::SetLimit
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamQuantity::SetLimit( int min, int max )
{
    min_ = min;
    max_ = max;
    if( spinBox_ )
    {
        spinBox_->setMinimum( min_ );
        spinBox_->setMaximum( max_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::SetSuffix
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamQuantity::SetSuffix( const QString& suffix )
{
    suffix_ = suffix;
    if( spinBox_ )
        spinBox_->setSuffix( suffix_ );
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::OnChanged
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void ParamQuantity::OnChanged( int value )
{
    modifier_ = value;
}

// -----------------------------------------------------------------------------
// Name: ParamQuantity::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void ParamQuantity::Visit( const actions::parameters::Quantity& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        spinBox_->setValue( param.GetValue() );
}
