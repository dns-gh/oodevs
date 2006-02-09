// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamNumericField.cpp $
// $Author: Age $
// $Modtime: 14/03/05 15:37 $
// $Revision: 2 $
// $Workfile: ParamNumericField.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParamNumericField.h"
#include <qvalidator.h>

// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( ASN1INT& asnInt, int nMin, int nMax, int nDefault, const std::string& strLabel, QWidget* pParent, bool bOptional )
: QHBox         ( pParent )
, Param_ABC ( bOptional )
, pAsnInt_      ( &asnInt )
, pAsnReal_     ( 0 )
{
    assert( nDefault >= nMin && nDefault <= nMax );

    this->setSpacing( 5 );
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    pEdit_ = new QLineEdit( QString::number( nDefault ), this );
    pEdit_->setValidator( new QIntValidator( nMin, nMax, pEdit_ ) );
}


// -----------------------------------------------------------------------------
// Name: ParamNumericField constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ParamNumericField::ParamNumericField( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent, bool bOptional )
: QHBox         ( pParent )
, Param_ABC ( bOptional )
, pAsnInt_      ( 0 )
, pAsnReal_     ( &asnReal )
{
    assert( rDefault >= rMin && rDefault <= rMax );

    this->setSpacing( 5 );
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    pEdit_ = new QLineEdit( QString::number( rDefault ), this );
    pEdit_->setValidator( new QDoubleValidator( rMin, rMax, 2, pEdit_ ) );
}


// -----------------------------------------------------------------------------
// Name: ParamNumericField destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ParamNumericField::~ParamNumericField()
{
}


// -----------------------------------------------------------------------------
// Name: ParamNumericField::WriteMsg
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ParamNumericField::WriteMsg( std::stringstream& strMsg )
{
    strMsg << pLabel_->text().latin1() << ": ";
    bool bOk = false;
    float rValue = pEdit_->text().toFloat( &bOk );
    assert( bOk );
    if( pAsnInt_ != 0 )
    {
        *pAsnInt_ = (int)rValue;
        strMsg << (int)rValue;
    }
    else
    {
        *pAsnReal_ = (float)rValue;
        strMsg << (float)rValue;
    }
}
