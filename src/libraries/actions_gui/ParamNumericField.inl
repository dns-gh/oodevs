// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions/Action_ABC.h"
#include "actions/Identifier.h"
#include "actions/Numeric.h"
#include <QtGui/qvalidator.h>
#include "InterfaceBuilder_ABC.h"

namespace actions
{
namespace gui
{

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
template< typename NumericType >
ParamNumericField< NumericType >::ParamNumericField( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : Param_ABC( builder, parameter )
    , pEdit_ ( 0 )
    , pLabel_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
template< typename NumericType >
ParamNumericField< NumericType >::~ParamNumericField()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename NumericType >
QWidget* ParamNumericField< NumericType >::BuildInterface( const QString& objectName, QWidget* parent )
{
    Param_ABC::BuildInterface( objectName, parent );
    QHBoxLayout* layout = new QHBoxLayout( group_ );
    pEdit_ = new QLineEdit( parent );
    connect( pEdit_, SIGNAL( textChanged( const QString& ) ), this, SLOT( Update() ) );
    layout->addWidget( pEdit_ );
    pEdit_->setPlaceholderText( "0" );
    double min = std::max< double >( parameter_.MinValue(), std::numeric_limits< NumericType >::min() );
    double max = std::min< double >( parameter_.MaxValue(), std::numeric_limits< NumericType >::max() );
    SetLimits( static_cast< NumericType >( min ), static_cast< NumericType >( max ) );
    pLabel_ = new QLabel( suffix_ );
    layout->addWidget( pLabel_ );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::SetSuffix
// Created: LDC 2012-09-05
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::SetSuffix( const QString& suffix )
{
    suffix_ = suffix;
    if( pLabel_ )
        pLabel_->setText( suffix );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::InternalCheckValidity
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
template< typename NumericType >
bool ParamNumericField< NumericType >::InternalCheckValidity() const
{
    if( !pEdit_ )
        return false;
    QString txt = pEdit_->text();
    int pos;
    return !txt.isEmpty() && pEdit_->validator()->validate( txt, pos ) == QValidator::Acceptable;
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    if( IsChecked() && !pEdit_->text().isEmpty() )
        action.AddParameter( *new actions::parameters::Numeric( parameter_, pEdit_->text().toFloat() ) );
    else
        action.AddParameter( *new actions::parameters::Numeric( parameter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::Visit( const actions::parameters::Identifier& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        pEdit_->setText( QString::number( param.GetValue() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Visit
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::Visit( const actions::parameters::Numeric& param )
{
    ActivateOptionalIfNeeded( param );
    if( param.IsSet() )
        pEdit_->setText( QString::number( param.GetValue() ) );
}

}
}
