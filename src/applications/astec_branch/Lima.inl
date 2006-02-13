//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Lima.inl $
// $Author: Ape $
// $Modtime: 19/04/04 10:51 $
// $Revision: 2 $
// $Workfile: Lima.inl $
//
//*****************************************************************************


// -----------------------------------------------------------------------------
// Name: Lima::GetLineType
/** @return 
*/
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
inline
TacticalLine_ABC::E_LineType Lima::GetLineType() const
{
    return TacticalLine_ABC::eLima;
}


//-----------------------------------------------------------------------------
// Name: Lima::GetTypeFunc
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
inline
E_FuncLimaType Lima::GetTypeFunc() const
{
    return nFuncType_;
}


//-----------------------------------------------------------------------------
// Name: Lima::SetTypeFunc
// Created: APE 2004-04-14
//-----------------------------------------------------------------------------
inline
void Lima::SetTypeFunc( E_FuncLimaType nTypeFunc )
{
    nFuncType_ = nTypeFunc;
}