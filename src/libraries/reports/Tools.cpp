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

#include "Tools.h"
#include "ENT/ENT_Tr.h"

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
QString tools::ToString(  sword::Location::Geometry nType )
{
    if( nType >= 0 && nType < 6 )
        return localisations[ nType ];
    return Unknown();
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
// Name: tools::DotationFamilyFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_DotationFamily tools::DotationFamilyFromString( const QString& type )
{
    return type.isNull() ? (E_DotationFamily)-1 : DotationFamilyFromString( std::string( type.ascii() ) );
}
