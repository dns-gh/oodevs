//*****************************************************************************
//
// $Created: FBD 03-01-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LifeBar.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_LifeBar.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_LifeBar::SetValue
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
inline
void MOS_LifeBar::SetValue( MT_Float rValue )
{
    rValue_ = rValue;
    setProgress( (int)rValue );
}
//-----------------------------------------------------------------------------
// Name: MOS_LifeBar::SetValueMax
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
inline
void MOS_LifeBar::SetValueMax( MT_Float rValueMax )
{
    rValueMax_ = rValueMax;
    setTotalSteps( rValueMax_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_LifeBar::SetValueMax
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
inline
void MOS_LifeBar::SetValueMin( MT_Float rValueMin )
{
    rValueMin_ = rValueMin;
}
