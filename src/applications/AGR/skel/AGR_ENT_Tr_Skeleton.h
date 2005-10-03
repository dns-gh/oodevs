// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-09 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Tr_Skeleton.h $
// $Author: Ape $
// $Modtime: 3/03/05 16:59 $
// $Revision: 3 $
// $Workfile: AGR_ENT_Tr_Skeleton.h $
//
// *****************************************************************************

#ifndef __ENT_Tr_Gen_h_
#define __ENT_Tr_Gen_h_

#include "ENT_Tr_ABC.h"
#include "ENT_Enums_Gen.h"

// =============================================================================
/** @class  AGR_ENT_Tr_Skeleton
    @brief  AGR_ENT_Tr_Skeleton
*/
// Created: APE 2005-02-09
// =============================================================================
class ENT_Tr
: public ENT_Tr_ABC
{
public:
    static void InitTranslations();

public:
    // ConvertFrom functions 
$ConvertFromFunctions$
    static const std::string& ConvertFromAutomataMission  ( E_AutomataMission  , E_Conversion = eToSim );
    static const std::string& ConvertFromUnitMission      ( E_UnitMission      , E_Conversion = eToSim );
    static const std::string& ConvertFromPopulationMission( E_PopulationMission, E_Conversion = eToSim );
    static const std::string& ConvertFromFragOrder        ( E_FragOrder        , E_Conversion = eToSim );

    // ConvertTo functions
$ConvertToFunctions$
    static E_AutomataMission   ConvertToAutomataMission  ( const std::string& );
    static E_UnitMission       ConvertToUnitMission      ( const std::string& );
    static E_PopulationMission ConvertToPopulationMission( const std::string& );
    static E_FragOrder         ConvertToFragOrder        ( const std::string& );

private:
    // Typedefs
$Typdefs$
    typedef ENT_Tr::converter<E_AutomataMission>   T_ConverterAutomataMission;
    typedef ENT_Tr::converter<E_UnitMission>       T_ConverterUnitMission;
    typedef ENT_Tr::converter<E_PopulationMission> T_ConverterPopulationMission;
    typedef ENT_Tr::converter<E_FragOrder>         T_ConverterFragOrder;

    // Converters
$Converters$
    static T_ConverterAutomataMission   automataMissionConverter_[];
    static T_ConverterUnitMission       unitMissionConverter_[];
    static T_ConverterPopulationMission populationMissionConverter_[];
    static T_ConverterFragOrder         fragOrderConverter_[];
};


#endif // __ENT_Tr_Gen_h_