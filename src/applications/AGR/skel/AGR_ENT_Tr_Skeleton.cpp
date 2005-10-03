// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-11 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Tr_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 3/03/05 16:59 $
// $Revision: 3 $
// $Workfile: AGR_ENT_Tr_Skeleton.cpp $
//
// *****************************************************************************

#include "ENT_Tr_Gen.h"


$Converters$


ENT_Tr::T_ConverterAutomataMission ENT_Tr::automataMissionConverter_[] =
{
$AutomataMissionConverterList$
    T_ConverterAutomataMission( "", "", (E_AutomataMission)-1  )
};


ENT_Tr::T_ConverterUnitMission ENT_Tr::unitMissionConverter_[] =
{
$UnitMissionConverterList$
    T_ConverterUnitMission( "", "", (E_UnitMission)-1  )
};

ENT_Tr::T_ConverterPopulationMission ENT_Tr::populationMissionConverter_[] =
{
$PopulationMissionConverterList$
    T_ConverterPopulationMission( "", "", (E_PopulationMission)-1  )
};

ENT_Tr::T_ConverterFragOrder ENT_Tr::fragOrderConverter_[] =
{
$FragOrderConverterList$
    T_ConverterFragOrder( "", "", (E_FragOrder)-1  )
};


//-----------------------------------------------------------------------------
// Name: ENT_Tr::InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
    $InitTr$
    InitTr( automataMissionConverter_ );
    InitTr( unitMissionConverter_ );
    InitTr( populationMissionConverter_ );
    InitTr( fragOrderConverter_ );
}


$ConvertFromFunctions$


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromAutomataMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomataMission( E_AutomataMission val, ENT_Tr_ABC::E_Conversion e )
{
   return ENT_Tr::InverseFindInConverter( automataMissionConverter_, val ,e );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitMission( E_UnitMission val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( unitMissionConverter_, val ,e );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPopulationMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationMission( E_PopulationMission val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( populationMissionConverter_, val ,e );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromFragOrder
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFragOrder( E_FragOrder val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( fragOrderConverter_, val, e );
}


$ConvertToFunctions$


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomataMission
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomataMission ENT_Tr::ConvertToAutomataMission( const std::string& strName )
{
   return ENT_Tr::FindInConverter( automataMissionConverter_, strName );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitMission
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitMission ENT_Tr::ConvertToUnitMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( unitMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationMission
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationMission ENT_Tr::ConvertToPopulationMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( populationMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToFragOrder
// Created: AGR
//-----------------------------------------------------------------------------
E_FragOrder ENT_Tr::ConvertToFragOrder( const std::string& strName )
{
    return ENT_Tr::FindInConverter( fragOrderConverter_, strName );
}

