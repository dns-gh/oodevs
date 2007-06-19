//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/tools.cpp $
// $Author: Age $
// $Modtime: 27/04/05 18:51 $
// $Revision: 21 $
// $Workfile: tools.cpp $
//
//*****************************************************************************

#include "gaming_pch.h"
#include "tools.h"
#include "ENT/ENT_Tr.h"

const QString limas[] = 
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

const QString localisations[] = 
{
    tools::translate( "Localisation", "Circle" ),
    tools::translate( "Localisation", "Ellipse" ),
    tools::translate( "Localisation", "Line" ),
    tools::translate( "Localisation", "Polygon" ),
    tools::translate( "Localisation", "Point" ),
    tools::translate( "Localisation", "Sector" ),
};

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
QString tools::ToString( E_FuncLimaType nType )
{
    if( nType >= 0 && nType < eLimaFuncNbr )
        return limas[ nType ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
QString tools::ToString( ASN1T_EnumLocationType nType )
{
    if( nType >= 0 && nType < 6 )
        return localisations[ nType ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::LimaTypeFromString
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
E_FuncLimaType tools::LimaTypeFromString( const QString& type )
{
    for( int i = 0; i < int( eLimaFuncNbr ); ++i )
        if( limas[i] == type )
            return (E_FuncLimaType)i;
    return (E_FuncLimaType)-1;
}

// -----------------------------------------------------------------------------
// Name: tools::LocationFromString
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
E_LocationType tools::LocationFromString( const QString& type )
{
    for( int i = 0; i < 6; ++i )
        if( type == localisations[i] )
            return (E_LocationType)i;
    return (E_LocationType)-1;
}

// -----------------------------------------------------------------------------
// Name: tools::AmmunitionFamilyFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_AmmunitionFamily tools::AmmunitionFamilyFromString( const QString& type )
{
    return ENT_Tr::ConvertToAmmunitionFamily( type.ascii() );
}

// -----------------------------------------------------------------------------
// Name: tools::DotationFamilyFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_DotationFamily tools::DotationFamilyFromString( const QString& type )
{
    return DotationFamilyFromString( std::string( type.ascii() ) );
}
