// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Tools.h"
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
// Name: Tools::FromString
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
E_FuncLimaType tools::FromString( const QString& type )
{
    for( int i = 0; i < int( eLimaFuncNbr ); ++i )
        if( limas[i] == type )
            return (E_FuncLimaType)i;
    return (E_FuncLimaType)-1;
}

// -----------------------------------------------------------------------------
// Name: tools::PopulationAttitudeFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_PopulationAttitude tools::PopulationAttitudeFromString( const QString& type )
{
    return ENT_Tr::ConvertToPopulationAttitude( type.ascii() );
}

// -----------------------------------------------------------------------------
// Name: tools::ObstacleTypeFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_ObstacleType tools::ObstacleTypeFromString( const QString& type )
{
    return ENT_Tr::ConvertToObstacleType( type.ascii() );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_ObstacleType type )
{
    return ENT_Tr::ConvertFromObstacleType( type, ENT_Tr::eToSim );
}

// -----------------------------------------------------------------------------
// Name: tools::ToXml
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
std::string tools::ToXml( E_PopulationAttitude type )
{
    return ENT_Tr::ConvertFromPopulationAttitude( type, ENT_Tr::eToSim );
}
