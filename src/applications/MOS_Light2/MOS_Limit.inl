//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Limit.inl $
// $Author: Ape $
// $Modtime: 19/04/04 10:50 $
// $Revision: 2 $
// $Workfile: MOS_Limit.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_Limit::GetLineType
/** @return 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_TacticalLine_ABC::E_LineType MOS_Limit::GetLineType() const
{
    return MOS_TacticalLine_ABC::eLimit;
}
