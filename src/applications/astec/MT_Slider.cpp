// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_Slider.cpp $
// $Author: Ape $
// $Modtime: 29/06/04 11:08 $
// $Revision: 1 $
// $Workfile: MT_Slider.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_Slider.h"
#include "moc_MT_Slider.cpp"

// -----------------------------------------------------------------------------
// Name: MT_Slider constructor
/** @param  nMinValue 
    @param  nMaxValue 
    @param  nPageStep 
    @param  nValue 
    @param  strLabelPostfix_ 
    @param  nPos 
    @param  nOrientation 
    @param  pParent 
    @param  szName 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_Slider::MT_Slider( int nMinValue, int nMaxValue, int nPageStep, int nValue, const QString& strLabelPostfix, E_LabelPos nLabelPos, Orientation nOrientation, QWidget* pParent, const char* szName /*= 0*/ )
    : QWidget           ( pParent, szName )
    , strLabelPostfix_  ( strLabelPostfix )
    , bFloat_           ( false )
{
    QBoxLayout* pLayout = 0;
    if( nOrientation == Qt::Horizontal )
        pLayout = new QHBoxLayout( this );
    else
        pLayout = new QVBoxLayout( this );

    pSlider_ = new QSlider( nMinValue, nMaxValue, nPageStep, nValue, nOrientation, this );
    pLabel_  = new QLabel( this );
    if( nOrientation == Qt::Vertical )
        pLabel_->setAlignment( Qt::AlignCenter );
    else
        pLabel_->setAlignment( nLabelPos == eTopLeft ? Qt::AlignLeft : Qt::AlignRight );

    if( nLabelPos == eTopLeft )
    {
        pLayout->addWidget( pLabel_ );
        pLayout->addWidget( pSlider_ );
    }
    else
    {
        pLayout->addWidget( pSlider_ );
        pLayout->addWidget( pLabel_ );
    }

    pLabel_->setText( QString::number( nValue ) + strLabelPostfix_ );
    connect( pSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
}


// -----------------------------------------------------------------------------
// Name: MT_Slider constructor
/** @param  nMinValue 
    @param  nMaxValue 
    @param  nPageStep 
    @param  nValue 
    @param  strLabelPostfix 
    @param  nLabelPos 
    @param  nOrientation 
    @param  pParent 
    @param  szName 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_Slider::MT_Slider( float nMinValue, float nMaxValue, float nPageStep, float nValue, const QString& strLabelPostfix, E_LabelPos nLabelPos, Orientation nOrientation, QWidget* pParent, const char* szName /*= 0*/ )
    : QWidget           ( pParent, szName )
    , strLabelPostfix_  ( strLabelPostfix )
    , bFloat_           ( true )
{
    QBoxLayout* pLayout = 0;
    if( nOrientation == Qt::Horizontal )
        pLayout = new QHBoxLayout( this );
    else
        pLayout = new QVBoxLayout( this );

    pSlider_ = new QSlider( nMinValue * 65536, nMaxValue * 65536, nPageStep * 65536, nValue * 65536, nOrientation, this );
    pLabel_  = new QLabel( this );
    if( nOrientation == Qt::Vertical )
        pLabel_->setAlignment( Qt::AlignCenter );
    else
        pLabel_->setAlignment( nLabelPos == eTopLeft ? Qt::AlignLeft : Qt::AlignRight );

    if( nLabelPos == eTopLeft )
    {
        pLayout->addWidget( pLabel_ );
        pLayout->addWidget( pSlider_ );
    }
    else
    {
        pLayout->addWidget( pSlider_ );
        pLayout->addWidget( pLabel_ );
    }

    pLabel_->setText( QString::number( nValue ) + strLabelPostfix_ );
    connect( pSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnValueChanged( int ) ) );
}


// -----------------------------------------------------------------------------
// Name: MT_Slider destructor
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_Slider::~MT_Slider()
{
}


// -----------------------------------------------------------------------------
// Name: MT_Slider::OnValueChanged
/** @param  nValue 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
void MT_Slider::OnValueChanged( int nValue )
{
    if( bFloat_ )
        pLabel_->setText( QString::number( nValue / 65536.0f ) + strLabelPostfix_ );
    else
        pLabel_->setText( QString::number( nValue ) + strLabelPostfix_ );

}
