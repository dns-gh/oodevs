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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_Slider.inl $
// $Author: Ape $
// $Modtime: 29/06/04 11:25 $
// $Revision: 1 $
// $Workfile: MT_Slider.inl $
//
// *****************************************************************************

#include <qslider.h>


// -----------------------------------------------------------------------------
// Name: MT_Slider::GetSlider
/** @return 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_INLINE
QSlider& MT_Slider::GetSlider()
{
    return *pSlider_;
}


// -----------------------------------------------------------------------------
// Name: MT_Slider::GetLabel
/** @return 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_INLINE
QLabel& MT_Slider::GetLabel()
{
    return *pLabel_;
}


// -----------------------------------------------------------------------------
// Name: MT_Slider::SetValue
/** @param  nValue 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_INLINE
void MT_Slider::SetValue( int nValue )
{
    assert( ! bFloat_ );
    pSlider_->setValue( nValue );
}


// -----------------------------------------------------------------------------
// Name: MT_Slider::SetValue
/** @param  rValue 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_INLINE
void MT_Slider::SetValue( float rValue )
{
    assert( bFloat_ );
    pSlider_->setValue( rValue * 65536 );
    pLabel_->setText( QString::number( rValue ) + strLabelPostfix_ );
}


// -----------------------------------------------------------------------------
// Name: MT_Slider::GetValue
/** @return 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
MT_INLINE
float MT_Slider::GetValue()
{
    if( bFloat_ )
        return pSlider_->value() / 65536.0f;
    else
        return pSlider_->value();
}
