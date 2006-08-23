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
QString tools::ToString( E_FuncLimaType nType )
{
    static const QString limas[] = 
    {
        tools::translate( "E_FuncLimaType", "LD" ),
        tools::translate( "E_FuncLimaType", "LCA" ),
        tools::translate( "E_FuncLimaType", "LC" ),
        tools::translate( "E_FuncLimaType", "LI" ),
        tools::translate( "E_FuncLimaType", "LO" ),
        tools::translate( "E_FuncLimaType", "LCAR" ),
        tools::translate( "E_FuncLimaType", "LR" ),
        tools::translate( "E_FuncLimaType", "LDM" ),
        tools::translate( "E_FuncLimaType", "LFM" ),
        tools::translate( "E_FuncLimaType", "LIA" ),
    };
    if( nType >= 0 && nType < eLimaFuncNbr )
        return limas[ nType ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
QString tools::ToString( ASN1T_EnumTypeLocalisation nType )
{
    static const QString localisations[] = 
    {
        tools::translate( "Localisation", "Cercle" ),
        tools::translate( "Localisation", "Ellipse" ),
        tools::translate( "Localisation", "Ligne" ),
        tools::translate( "Localisation", "Polygone" ),
        tools::translate( "Localisation", "Point" ),
        tools::translate( "Localisation", "Secteur" ),
    };
    if( nType >= 0 && nType < 6 )
        return localisations[ nType ];
    return Unknown();
}
