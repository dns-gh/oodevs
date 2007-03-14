// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
ParamNumericField::ParamNumericField( QObject* parent, ASN1INT& asn, const QString& name, int min /*= 0*/, int max /*= 9999*/ )
    : Param_ABC( name )
    , int_( &asn )
    , real_( 0 )
    , validator_( new QIntValidator( min, max, parent ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( QObject* parent, ASN1REAL& asn, const QString& name, float min /*= 0.*/, float max /*= 9999.*/, int precision /*= 2*/ )
    : Param_ABC( name )
    , int_ ( 0 )
    , real_( &asn )
    , validator_( new QDoubleValidator( min, max, precision, parent ) )
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
    QHBox* box = new QHBox( parent );
    box->setSpacing( 5 );
    pLabel_ = new gui::RichLabel( GetName(), box );
    pLabel_->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );

    pEdit_ = new QLineEdit( "0", box );
    pEdit_->setValidator( validator_ );
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
// Name: ParamNumericField::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamNumericField::Commit()
{
    const float rValue = pEdit_->text().toFloat();
    if( int_ )
        *int_ = (int)rValue;
    else
        *real_ = (float)rValue;
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::CommitTo
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamNumericField::CommitTo( ASN1INT& asn )
{
    asn = pEdit_->text().toInt();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::CommitTo
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
void ParamNumericField::CommitTo( ASN1REAL& asn )
{
    asn = pEdit_->text().toFloat();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Show
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Show()
{
    pLabel_->parentWidget()->show();
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField::Hide
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamNumericField::Hide()
{
    pLabel_->parentWidget()->hide();
}
