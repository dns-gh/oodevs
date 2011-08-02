// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Kernel_Tools_h_
#define __Kernel_Tools_h_

#include "Types.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

// =============================================================================
/** @class  Tools
    @brief  Crappy translation tools
*/
// Created: AGE 2006-03-15
// =============================================================================
namespace tools
{
    QString translate( const char* context, const char* what );
    QString findTranslation( const char* context, const char* what );

    QString ToString( kernel::E_TroopHealthState nState );
    QString ToString( kernel::E_PerceptionResult nResult );
    QString ToString( kernel::E_DayLightingType lighting );
    QString ToString( kernel::E_NightLightingType lighting );
    QString ToString( E_WeatherType weather );
    QString ToString( kernel::E_FuncLimaType nType );
    QString ToShortString( kernel::E_FuncLimaType nType );

    QString ToString( E_PopulationAttitude nType );
    QString ToString( E_DemolitionTargetType nType );
    QString ToString( E_UnitPosture nType );
    QString ToString( E_OperationalStatus nType );
    QString ToString( E_ForceRatioStatus nType );
    QString ToString( E_Roe nType );
    QString ToString( E_PopulationRoe nType );
    QString ToString( E_MeetingEngagementStatus nType );
    QString ToString( E_FireAvailability nType );
    QString ToString( E_NatureLevel nType );
    QString ToString( E_UnitExperience nType );
    QString ToString( E_UnitMorale nType );
    QString ToString( E_UnitStress nType );
    QString ToString( E_UnitTiredness nType );
    QString ToString( E_HumanWound nType );
    QString ToString( E_LogSupplyHandlingStatus nType );
    QString ToString( E_LogMedicalHandlingStatus nType );
    QString ToString( E_LogMaintenanceHandlingStatus nType );
    QString ToString( E_DotationFamily nType );
    QString ToString( E_LocationType nType );

    QString Unknown();

    QString ToDisplayedString( E_LightingType lighting );
    QString ToDisplayedString( E_WeatherType weather );
    QString ToDisplayedString( kernel::E_DayLightingType lighting );
    QString ToDisplayedString( kernel::E_NightLightingType lighting );

    const char* GetXmlSection( E_UnitPosture nPosture );
    const char* GetXmlSection( E_WeatherType nWeatherType );
    const char* GetXmlSection( kernel::E_DayLightingType nLightingType );
    const char* GetXmlSection( kernel::E_NightLightingType nLightingType );

    E_DotationFamily        DotationFamilyFromString( const std::string& type );
    E_NatureLevel           NatureLevelFromString   ( const std::string& type );
    E_LocationType          LocationFromString      ( const std::string& type );

    kernel::E_FuncLimaType  LimaTypeFromShortString ( const QString& type );
    kernel::E_FuncLimaType  LimaTypeFromXmlString   ( const QString& type );
    QString                 LimaTypeShortToXmlString( const QString& shortType );
}

#endif // __Kernel_Tools_h_
