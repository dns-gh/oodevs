// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Tools.h"
#include "ENT/ENT_Tr.h"
#include <qapplication.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: tools::translate
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
QString tools::translate( const char* context, const char* what )
{
    if ( qApp )
	    return qApp->translate( context, what );
    else
    	return QString::fromLatin1( what );
}

// -----------------------------------------------------------------------------
// Name: tools::Unknown
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
QString tools::Unknown()
{
    static const QString unknown = tools::translate( "tools", "Unknown" );
    return unknown;
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
QString tools::ToString( E_TroopHealthState nState )
{
    static const QString healthStates[] = 
    {
        tools::translate( "E_TroopHealthState", "Total" ),
        tools::translate( "E_TroopHealthState", "Operational" ),
        tools::translate( "E_TroopHealthState", "Dead" ),
        tools::translate( "E_TroopHealthState", "Injured" ),
        tools::translate( "E_TroopHealthState", "Mentally injured" ),
        tools::translate( "E_TroopHealthState", "Contaminated" ),
        tools::translate( "E_TroopHealthState", "Under treatment " ),
        tools::translate( "E_TroopHealthState", "Assigned to maintenance" )
    };
    if( nState >= 0 && nState < eTroopHealthStateNbrStates )
        return healthStates[ nState ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
QString tools::ToString( E_PerceptionResult nResult )
{
    static const QString perceptions[] = 
    {
        tools::translate( "E_PerceptionResult", "Not seen" ),
        tools::translate( "E_PerceptionResult", "Detection" ),
        tools::translate( "E_PerceptionResult", "Recognition" ),
        tools::translate( "E_PerceptionResult", "Identification" ),
    };
    if( nResult >= 0 && nResult < 4 )
        return perceptions[ nResult ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
QString tools::ToString( kernel::E_LightingType lighting )
{
    static const QString lightings[] = 
    {
        tools::translate( "E_PerceptionResult", "JourSansNuage" ),
        tools::translate( "E_PerceptionResult", "JourPeuNuageux" ),
        tools::translate( "E_PerceptionResult", "JourMoyennementNuageux" ),
        tools::translate( "E_PerceptionResult", "JourAssezNuageux" ),
        tools::translate( "E_PerceptionResult", "JourTresNuageux" ),
        tools::translate( "E_PerceptionResult", "NuitPleineLune" ),
        tools::translate( "E_PerceptionResult", "NuitTroisQuartDeLune" ),
        tools::translate( "E_PerceptionResult", "NuitDemiLune" ),
        tools::translate( "E_PerceptionResult", "NuitQuartDeLune" ),
        tools::translate( "E_PerceptionResult", "NuitNouvelleLune" ),
        tools::translate( "E_PerceptionResult", "Eclairant" )
    };
    if( lighting >= 0 && lighting < eNbrLightingType )
        return lightings[ lighting ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
QString tools::ToString( kernel::E_WeatherType weather )
{
    static const QString weathers[] = 
    {
        tools::translate( "E_PerceptionResult", "PasDePrecipitation" ),
        tools::translate( "E_PerceptionResult", "TempeteDeSable" ),
        tools::translate( "E_PerceptionResult", "Brouillard" ),
        tools::translate( "E_PerceptionResult", "Crachin" ),
        tools::translate( "E_PerceptionResult", "Pluie" ),
        tools::translate( "E_PerceptionResult", "Neige" ),
        tools::translate( "E_PerceptionResult", "Fumigene" )
    };
    if( weather >= 0 && weather < eNbrWeatherType )
        return weathers[ weather ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::GetXmlSection
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
const char* tools::GetXmlSection( E_UnitPosture nPosture )
{
    switch( nPosture )
    {
        case eUnitPosture_PostureMouvement:         return "Mouvement";
        case eUnitPosture_PostureMouvementDiscret:  return "MouvementDiscret";
        case eUnitPosture_PostureArret:             return "Arret";
        case eUnitPosture_PosturePosteReflexe:      return "PosteReflexe";
        case eUnitPosture_PosturePoste:             return "Poste";
        case eUnitPosture_PosturePosteAmenage:      return "PosteAmenage";
        case eUnitPosture_PosturePostePrepareGenie: return "PostePrepareGenie";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::GetXmlSection
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
const char* tools::GetXmlSection( E_WeatherType weather )
{
    switch( weather  )
    {
        case eWeatherTypeNone:            return "PasDePrecipitation";
        case eWeatherTypeRainOrSnowStorm: return "TempeteDeSable";
        case eWeatherTypeFog:             return "Brouillard";
        case eWeatherTypeDrizzle:         return "Crachin";
        case eWeatherTypeRain:            return "Pluie";
        case eWeatherTypeSnow:            return "Neige";
        case eWeatherTypeSmoke:           return "Fumigene";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::GetXmlSection
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
const char* tools::GetXmlSection( E_LightingType weather )
{
    switch( weather  )
    {
        case eLightingJourSansNuage:            return "JourSansNuage";
        case eLightingJourPeuNuageux:           return "JourPeuNuageux";
        case eLightingJourMoyennementNuageux:   return "JourMoyennementNuageux";
        case eLightingJourAssezNuageux:         return "JourAssezNuageux";
        case eLightingJourTresNuageux:          return "JourTresNuageux";
        case eLightingNuitPleineLune:           return "NuitPleineLune";
        case eLightingNuitTroisQuartDeLune:     return "NuitTroisQuartDeLune";
        case eLightingNuitDemiLune:             return "NuitDemiLune";
        case eLightingNuitQuartDeLune:          return "NuitQuartDeLune";
        case eLightingNuitNouvelleLune:         return "NuitNouvelleLune";
        case eLightingEclairant:                return "Eclairant";
        default:
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_PopulationAttitude nType )
{
    return ENT_Tr::ConvertFromPopulationAttitude( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_ObstacleType nType )
{
    return ENT_Tr::ConvertFromObstacleType( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitPosture nType )
{
    return ENT_Tr::ConvertFromUnitPosture( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_OperationalStatus nType )
{
    return ENT_Tr::ConvertFromOperationalStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_ForceRatioStatus nType )
{
    return ENT_Tr::ConvertFromForceRatioStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_Roe nType )
{
    return ENT_Tr::ConvertFromRoe( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_PopulationRoe nType )
{
    return ENT_Tr::ConvertFromPopulationRoe( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_MeetingEngagementStatus nType )
{
    return ENT_Tr::ConvertFromMeetingEngagementStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_FireAvailability nType )
{
    return ENT_Tr::ConvertFromFireAvailability( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_NatureLevel nType )
{
    return ENT_Tr::ConvertFromNatureLevel( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitExperience nType )
{
    return ENT_Tr::ConvertFromUnitExperience( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitMorale nType )
{
    return ENT_Tr::ConvertFromUnitMorale( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitTiredness nType )
{
    return ENT_Tr::ConvertFromUnitTiredness( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanWound nType )
{
    return ENT_Tr::ConvertFromHumanWound( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_LogSupplyHandlingStatus nType )
{
    return ENT_Tr::ConvertFromLogSupplyHandlingStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_LogMedicalHandlingStatus nType )
{
    return ENT_Tr::ConvertFromLogMedicalHandlingStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_LogMaintenanceHandlingStatus nType )
{
    return ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( nType, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2008-02-05
// -----------------------------------------------------------------------------
QString tools::ToString( E_DotationFamily nType )
{
    return ENT_Tr::ConvertFromDotationFamily( nType ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ObjectTypeFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_ObjectType tools::ObjectTypeFromString( const std::string& type )
{
    return ENT_Tr::ConvertToObjectType( type );
}

// -----------------------------------------------------------------------------
// Name: tools::DotationFamilyFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_DotationFamily tools::DotationFamilyFromString( const std::string& type )
{
    return ENT_Tr::ConvertToDotationFamily( type );
}

// -----------------------------------------------------------------------------
// Name: tools::NatureLevelFromString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
E_NatureLevel tools::NatureLevelFromString( const std::string& type )
{
    return ENT_Tr::ConvertToNatureLevel( type );
}

// -----------------------------------------------------------------------------
// Name: tools::TranslateObjectType
// Created: AGE 2008-02-06
// -----------------------------------------------------------------------------
QString tools::TranslateObjectType( const std::string& xmlType )
{
    return ENT_Tr::ConvertFromObjectType( 
                ENT_Tr::ConvertToObjectType( xmlType ), ENT_Tr::eToTr
                ).c_str();
}
