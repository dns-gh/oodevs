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
#include <qvalidator.h>

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( QWidget* parent, ASN1INT& asn, const std::string& label )
    : QHBox( parent )
    , int_( &asn )
    , real_( 0 )
{
    setSpacing( 5 );
    pLabel_ = new QLabel( label.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    pEdit_ = new QLineEdit( QString::number( 0 ), this );
    pEdit_->setValidator( new QIntValidator( 0, 9999, pEdit_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( QWidget* parent, ASN1REAL& asn, const std::string& label )
    : QHBox( parent )
    , int_ ( 0 )
    , real_( &asn )
{
    setSpacing( 5 );
    pLabel_ = new QLabel( label.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    pEdit_ = new QLineEdit( QString::number( 0. ), this );
    pEdit_->setValidator( new QDoubleValidator( 0., 9999., 2, pEdit_ ) );
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
