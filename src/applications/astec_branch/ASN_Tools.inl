//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ASN_Tools.inl $
// $Author: Ape $
// $Modtime: 18/02/04 16:24 $
// $Revision: 1 $
// $Workfile: ASN_Tools.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: ASN_Tools::BuildAsnGDH
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
inline
void ASN_Tools::BuildAsnGDH( ASN1T_GDH& asnGDH )
{
    asnGDH.qualificatif = EnumGDH_Qualificatif::at;
}