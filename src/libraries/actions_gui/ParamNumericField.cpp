// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamNumericField.h"
#include "actions/Action_ABC.h"
#include "actions/Numeric.h"
#include "clients_gui/RichLabel.h"
#include <QtGui/qvalidator.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( const kernel::OrderParameter& parameter, bool isReal )
    : Param_ABC( parameter.GetName().c_str(), parameter.IsOptional() )
    , parameter_( parameter )
    , isReal_( isReal )
    , pEdit_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::~ParamNumericField()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
QWidget* ParamNumericField::BuildInterface( QWidget* parent )
{
    Param_ABC::BuildInterface( parent );
    QVBoxLayout* layout = new QVBoxLayout( group_ );
    pEdit_ = new QLineEdit( parent );
    layout->addWidget( pEdit_ );
    pEdit_->setPlaceholderText( "0" );
    SetLimits( 0.f, 99999.f );
    return group_;
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::SetLimits
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamNumericField::SetLimits( float min, float max )
{
    QValidator* validator;
    if( isReal_ )
        validator = new QDoubleValidator( min, max, 2, pEdit_ );
    else
        validator = new QIntValidator( int( min ), int( max ), pEdit_ );
    pEdit_->setValidator( validator );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::InternalCheckValidity
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
bool ParamNumericField::InternalCheckValidity() const
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
void ParamNumericField::CommitTo( actions::ParameterContainer_ABC& action ) const
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
void ParamNumericField::Show()
{
    if( group_ )
        group_->show();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Hide()
{
    if( group_ )
        group_->hide();
}
