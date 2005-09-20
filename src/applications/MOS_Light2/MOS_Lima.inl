//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Lima.inl $
// $Author: Ape $
// $Modtime: 19/04/04 10:51 $
// $Revision: 2 $
// $Workfile: MOS_Lima.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_Lima::GetLineType
/** @return 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
inline
MOS_TacticalLine_ABC::E_LineType MOS_Lima::GetLineType() const
{
    return MOS_TacticalLine_ABC::eLima;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::GetTypeFunc
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
inline
E_FuncLimaType MOS_Lima::GetTypeFunc() const
{
    return nFuncType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Lima::SetTypeFunc
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
inline
void MOS_Lima::SetTypeFunc( E_FuncLimaType nTypeFunc )
{
    nFuncType_ = nTypeFunc;
}