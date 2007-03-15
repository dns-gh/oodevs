// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamNumericField.h"
#include "clients_gui/RichLabel.h"
#include <qvalidator.h>

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( const QString& name, bool isReal )
    : Param_ABC( name )
    , isReal_( isReal )
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
void ParamNumericField::BuildInterface( QWidget* parent )
{
    pLabel_ = new gui::RichLabel( GetName(), parent );
    pLabel_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    pEdit_ = new QLineEdit( "0", parent );
    SetLimits( 0.f, 99999.f );
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
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamNumericField::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_aReal;
    asn.value.u.aReal = pEdit_->text().toFloat();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamNumericField::CommitTo( ASN1REAL& asn ) const
{
    asn = pEdit_->text().toFloat();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Show()
{
    pLabel_->show();
    pEdit_->show();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Hide()
{
    pLabel_->hide();
    pEdit_->hide();
}
