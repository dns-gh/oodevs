// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions/Action_ABC.h"
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
    : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter )
    , pEdit_ ( 0 )
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
QWidget* ParamNumericField< NumericType >::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    pEdit_ = new QLineEdit( parent );
    layout->addWidget( pEdit_ );
    pEdit_->setPlaceholderText( "0" );
    SetLimits( static_cast< NumericType >( 0.f ), static_cast< NumericType >( 100000.f ) );
    return group_;
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
// Name: ParamNumericField::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::Show()
{
    if( group_ )
        group_->show();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
template< typename NumericType >
void ParamNumericField< NumericType >::Hide()
{
    if( group_ )
        group_->hide();
}

}
}
