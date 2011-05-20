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
#include <qvalidator.h>

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( const kernel::OrderParameter& parameter, bool isReal )
    : Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , isReal_( isReal )
    , pLabel_( 0 )
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
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new ::gui::RichLabel( GetName(), box );
    pLabel_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    pEdit_ = new QLineEdit( "0", box );
    SetLimits( 0.f, 99999.f );
    box->setStretchFactor( pEdit_, 1 );
    return box;
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
// Name: ParamNumericField::CheckValidity
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
bool ParamNumericField::CheckValidity()
{
    int pos;
    QString txt = pEdit_->text();
    if( pEdit_->validator()->validate( txt, pos ) != QValidator::Acceptable )
    {
        pLabel_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamNumericField::CommitTo( actions::ParameterContainer_ABC& action ) const
{
    action.AddParameter( *new actions::parameters::Numeric( parameter_, pEdit_->text().toFloat() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Show()
{
    if( pLabel_ && pEdit_ )
    {
        pLabel_->show();
        pEdit_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Hide()
{
    if( pLabel_ && pEdit_ )
    {
        pLabel_->hide();
        pEdit_->hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamNumericField::IsOptional() const
{
    return parameter_.IsOptional();
}
