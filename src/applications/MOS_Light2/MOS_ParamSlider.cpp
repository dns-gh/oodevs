// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamSlider.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 3 $
// $Workfile: MOS_ParamSlider.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamSlider.h"
#include "moc_MOS_ParamSlider.cpp"

// -----------------------------------------------------------------------------
// Name: MOS_ParamSlider constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
MOS_ParamSlider::MOS_ParamSlider( ASN1INT& asnInt, int nMin, int nMax, int nDefault, const std::string& strLabel, QWidget* pParent, bool bOptional )
    : QHBox         ( pParent )
    , MOS_Param_ABC ( bOptional )
    , pAsnInt_      ( &asnInt )
    , pAsnReal_     ( 0 )
{
    assert( nDefault >= nMin && nDefault <= nMax );

    this->setSpacing( 5 );
    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    pValueLabel_ = new QLabel( QString::number( nDefault ), this );
    pValueLabel_->setAlignment( AlignVCenter | AlignCenter );
    pValueLabel_->setMinimumWidth( 35 );

    pSlider_ = new QSlider( nMin, nMax, 1, nDefault, Qt::Horizontal, this );
    connect( pSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamSlider constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
MOS_ParamSlider::MOS_ParamSlider( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent, bool bOptional )
    : QHBox         ( pParent )
    , MOS_Param_ABC ( bOptional )
    , pAsnInt_      ( 0 )
    , pAsnReal_     ( &asnReal )
{
    assert( rDefault >= rMin && rDefault <= rMax );

    pLabel_ = new QLabel( strLabel.c_str(), this );
    pLabel_->setAlignment( AlignVCenter | AlignLeft );

    int nDefault = rDefault * 65536;
    int nMin = rMin * 65536;
    int nMax = rMax * 65536;

    pValueLabel_ = new QLabel( QString::number( (float)nDefault / 65536.0f ), this );
    pValueLabel_->setAlignment( AlignVCenter | AlignCenter );
    pValueLabel_->setMinimumWidth( 35 );

    pSlider_ = new QSlider( nMin, nMax, 1, nDefault, Qt::Horizontal, this );
    connect( pSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamSlider destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
MOS_ParamSlider::~MOS_ParamSlider()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamSlider::WriteMsg
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_ParamSlider::WriteMsg( std::stringstream& strMsg )
{
    strMsg << pLabel_->text().latin1() << ": ";
    if( pAsnInt_ != 0 )
    {
        *pAsnInt_ = pSlider_->value();
        strMsg << pSlider_->value();
    }
    else
    {
        *pAsnReal_ = (float)(pSlider_->value()) / 65536.0f;
        strMsg << (float)(pSlider_->value()) / 65536.0f;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamSlider::OnValueChanged
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
void MOS_ParamSlider::OnValueChanged( int nValue )
{
    if( pAsnInt_ != 0 )
        pValueLabel_->setText( QString::number( nValue ) );
    else
        pValueLabel_->setText( QString::number( (float)nValue / 65536.0f ) );
}
