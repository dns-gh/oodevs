//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Tools.cpp $
// $Author: Age $
// $Modtime: 27/04/05 18:51 $
// $Revision: 21 $
// $Workfile: Tools.cpp $
//
//*****************************************************************************

#include "gaming_pch.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
const char* tools::ToString( E_FuncLimaType nType )
{
    switch( nType )
    {
        case eLimaFuncLD:     return "LD";
        case eLimaFuncLCA:    return "LCA";
        case eLimaFuncLC:     return "LC";
        case eLimaFuncLI:     return "LI";
        case eLimaFuncLO:     return "LO";
        case eLimaFuncLCAR:   return "LCAR";
        case eLimaFuncLR:     return "LR";
        case eLimaFuncLDM:    return "LDM";
        case eLimaFuncLFM:    return "LFM";
        case eLimaFuncLIA:    return "LIA";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
const char* tools::ToString( ASN1T_EnumTypeLocalisation nType )
{
    switch( nType )
    {
        case EnumTypeLocalisation::circle:   return "cercle";
        case EnumTypeLocalisation::ellipse:  return "ellipse";
        case EnumTypeLocalisation::line:     return "ligne";
        case EnumTypeLocalisation::polygon:  return "polygone";
        case EnumTypeLocalisation::point:    return "point";
        case EnumTypeLocalisation::sector:   return "secteur";
        default:
            return "Unknown";
    }
}
