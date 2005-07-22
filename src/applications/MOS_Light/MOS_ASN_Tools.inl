//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ASN_Tools.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_ASN_Tools.inl $
//
//*****************************************************************************

#include "MOS_App.h"

//-----------------------------------------------------------------------------
// Name: MOS_ASN_Tools::BuildAsnGDH
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
inline
void MOS_ASN_Tools::BuildAsnGDH( ASN1T_GDH& asnGDH )
{
    asnGDH.datation     = MOS_App::GetApp().GetTime();
    asnGDH.qualificatif = EnumGDH_Qualificatif::at;
}