//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ASN_Tools.inl $
// $Author: Ape $
// $Modtime: 18/02/04 16:24 $
// $Revision: 1 $
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