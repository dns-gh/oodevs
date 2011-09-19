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
#include <QtGui/qapplication.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: tools::translate
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
QString tools::translate( const char* context, const char* what )
{
    if( qApp )
        return qApp->translate( context, what );
    else
        return QString::fromLatin1( what );
}

// -----------------------------------------------------------------------------
// Name: tools::findTranslation
// Created: SBO 2011-05-11
// -----------------------------------------------------------------------------
QString tools::findTranslation( const char* context, const char* what )
{
    return tools::translate( context, what );
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
        tools::translate( "E_TroopHealthState", "Assigned to maintenance" ),
        tools::translate( "E_TroopHealthState", "Wounded not evacuated" )
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
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToString( E_DayLightingType lighting )
{
    static const QString lightings[] =
    {
        tools::translate( "E_DayLightingType", "JourSansNuage" ),
        tools::translate( "E_DayLightingType", "JourPeuNuageux" ),
        tools::translate( "E_DayLightingType", "JourMoyennementNuageux" ),
        tools::translate( "E_DayLightingType", "JourAssezNuageux" ),
        tools::translate( "E_DayLightingType", "JourTresNuageux" ),
        tools::translate( "E_DayLightingType", "Eclairant" )
    };
    if( lighting >= 0 && lighting < eNbrDayLightingType )
        return lightings[ lighting ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToString( E_NightLightingType lighting )
{
    static const QString lightings[] =
    {
        tools::translate( "E_NightLightingType", "NuitPleineLune" ),
        tools::translate( "E_NightLightingType", "NuitTroisQuartDeLune" ),
        tools::translate( "E_NightLightingType", "NuitDemiLune" ),
        tools::translate( "E_NightLightingType", "NuitQuartDeLune" ),
        tools::translate( "E_NightLightingType", "NuitNouvelleLune" ),
        tools::translate( "E_NightLightingType", "Eclairant" )
    };
    if( lighting >= 0 && lighting < eNbrNightLightingType )
        return lightings[ lighting ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
QString tools::ToString( E_WeatherType weather )
{
    static const QString weathers[] =
    {
        tools::translate( "E_WeatherType", "PasDePrecipitation" ),
        tools::translate( "E_WeatherType", "TempeteDeSable" ),
        tools::translate( "E_WeatherType", "Brouillard" ),
        tools::translate( "E_WeatherType", "Crachin" ),
        tools::translate( "E_WeatherType", "Pluie" ),
        tools::translate( "E_WeatherType", "Neige" ),
        tools::translate( "E_WeatherType", "Fumigene" )
    };
    if( weather >= 0 && weather < eNbrWeatherType )
        return weathers[ weather ];
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToDisplayedString
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
QString tools::ToDisplayedString( E_LightingType lighting )
{
    return ENT_Tr::ConvertFromLightingType( lighting, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToDisplayedString
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToDisplayedString( E_WeatherType weather )
{
    return ENT_Tr::ConvertFromWeatherType( weather, ENT_Tr::eToTr ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToDisplayedString
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToDisplayedString( E_DayLightingType lighting )
{
    switch( lighting )
    {
    case eDayLightingJourSansNuage:
    default:
        return tools::translate( "tools", "No clouds" );
    case eDayLightingJourPeuNuageux:
        return tools::translate( "tools", "Slightly cloudy" );
    case eDayLightingJourMoyennementNuageux:
        return tools::translate( "tools", "Moderately cloudy" );
    case eDayLightingJourAssezNuageux:
        return tools::translate( "tools", "Quite cloudy" );
    case eDayLightingJourTresNuageux:
        return tools::translate( "tools", "Very cloudy" );
    }
}

// -----------------------------------------------------------------------------
// Name: tools::ToDisplayedString
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToDisplayedString( E_NightLightingType lighting )
{
    switch( lighting )
    {
    case eNightLightingNuitPleineLune:
    default:
        return tools::translate( "tools", "Full moon" );
    case eNightLightingNuitTroisQuartDeLune:
        return tools::translate( "tools", "Three quarter moon" );
    case eNightLightingNuitDemiLune:
        return tools::translate( "tools", "Half moon" );
    case eNightLightingNuitQuartDeLune:
        return tools::translate( "tools", "Quarter moon" );
    case eNightLightingNuitNouvelleLune:
        return tools::translate( "tools", "New moon" );
    }
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
        case eWeatherType_None:      return "PasDePrecipitation";
        case eWeatherType_SandStorm: return "TempeteDeSable";
        case eWeatherType_Fog:       return "Brouillard";
        case eWeatherType_Drizzle:   return "Crachin";
        case eWeatherType_Rain:      return "Pluie";
        case eWeatherType_Snow:      return "Neige";
        case eWeatherType_Smoke:     return "Fumigene";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::GetXmlSection
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
const char* tools::GetXmlSection( E_DayLightingType nLightingType )
{
    switch( nLightingType )
    {
        case eDayLightingJourSansNuage:            return "JourSansNuage";
        case eDayLightingJourPeuNuageux:           return "JourPeuNuageux";
        case eDayLightingJourMoyennementNuageux:   return "JourMoyennementNuageux";
        case eDayLightingJourAssezNuageux:         return "JourAssezNuageux";
        case eDayLightingJourTresNuageux:          return "JourTresNuageux";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::GetXmlSection
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
const char* tools::GetXmlSection( E_NightLightingType nLightingType )
{
    switch( nLightingType )
    {
        case eNightLightingNuitPleineLune:           return "NuitPleineLune";
        case eNightLightingNuitTroisQuartDeLune:     return "NuitTroisQuartDeLune";
        case eNightLightingNuitDemiLune:             return "NuitDemiLune";
        case eNightLightingNuitQuartDeLune:          return "NuitQuartDeLune";
        case eNightLightingNuitNouvelleLune:         return "NuitNouvelleLune";
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
QString tools::ToString( E_DemolitionTargetType nType )
{
    return ENT_Tr::ConvertFromDemolitionTargetType( nType, ENT_Tr::eToTr ).c_str();
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
// Name: Tools::ToString
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitStress nType )
{
    return ENT_Tr::ConvertFromUnitStress( nType, ENT_Tr::eToTr ).c_str();
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
// Name: tools::ToString
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_LocationType nType )
{
    return ENT_Tr::ConvertFromLocationType( nType ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanRank nType, ENT_Tr_ABC::E_Conversion conversion /* = ENT_Tr_ABC::eToTr */ )
{
    return ENT_Tr::ConvertFromHumanRank( nType, conversion ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanState nType, ENT_Tr_ABC::E_Conversion conversion /* = ENT_Tr_ABC::eToTr */ )
{
    return ENT_Tr::ConvertFromHumanState( nType, conversion ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanLocation nType, ENT_Tr_ABC::E_Conversion conversion /* = ENT_Tr_ABC::eToTr */ )
{
    return ENT_Tr::ConvertFromHumanLocation( nType, conversion ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_EquipmentState nType, ENT_Tr_ABC::E_Conversion conversion /* = ENT_Tr_ABC::eToTr */ )
{
    return ENT_Tr::ConvertFromEquipmentState( nType, conversion ).c_str();
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_InjuriesSeriousness nType, ENT_Tr_ABC::E_Conversion conversion /* = ENT_Tr_ABC::eToTr */ )
{
    return ENT_Tr::ConvertFromInjuriesSeriousness( nType, conversion ).c_str();
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
// Name: Tools::tools
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
E_LocationType tools::LocationFromString( const std::string& type )
{
    return ENT_Tr::ConvertToLocationType( type );
}

namespace
{
    // $$$$ SBO 2008-08-11: create a phase line type into static model
    struct PhaseLineType
    {
        PhaseLineType( E_FuncLimaType type, const QString& fullName, const QString& shortName, const QString& xmlName )
            : type_( type )
            , fullName_( fullName )
            , shortName_( shortName )
            , xmlName_( xmlName )
        {}
        E_FuncLimaType type_;
        QString fullName_;
        QString shortName_;
        QString xmlName_;
    };

    std::vector< PhaseLineType > InitializePhaseLines()
    {
        std::vector< PhaseLineType > lines;
        // $$$$ SBO 2008-08-11: LD
        lines.push_back( PhaseLineType( eLimaFuncLD, tools::translate( "E_FuncLimaType", "Line of departure (LD)" ), tools::translate( "E_FuncLimaType", "LD" ), "LD" ) );
        // $$$$ SBO 2008-08-11: not operational (should probably be "phase line")
        lines.push_back( PhaseLineType( eLimaFuncLCA, tools::translate( "E_FuncLimaType", "Attitude change line (ACL)" ), tools::translate( "E_FuncLimaType", "ACL" ), "LCA" ) );
        // $$$$ SBO 2008-08-11: coordination should be "phase line"
        // $$$$ SBO 2008-08-11: or FSCL "Fire support coordination line"
        lines.push_back( PhaseLineType( eLimaFuncLC, tools::translate( "E_FuncLimaType", "Coordination line (CL)" ), tools::translate( "E_FuncLimaType", "CL" ), "LC" ) );
        // $$$$ SBO 2008-08-11: not operational (should probably be "phase line")
        lines.push_back( PhaseLineType( eLimaFuncLI, tools::translate( "E_FuncLimaType", "Denial line (DL)" ), tools::translate( "E_FuncLimaType", "DL" ), "LI" ) );
        // $$$$ SBO 2008-08-11: should be "phase line"
        lines.push_back( PhaseLineType( eLimaFuncLO, tools::translate( "E_FuncLimaType", "Objective line (OL)" ), tools::translate( "E_FuncLimaType", "OL" ), "LO" ) );
        // $$$$ SBO 2008-08-11: not operational
        lines.push_back( PhaseLineType( eLimaFuncLCAR, tools::translate( "E_FuncLimaType", "Blocking line (BL)" ), tools::translate( "E_FuncLimaType", "BL" ), "LCAR" ) );
        // $$$$ SBO 2008-08-11: LRCL
        lines.push_back( PhaseLineType( eLimaFuncLR, tools::translate( "E_FuncLimaType", "Handover line (HL)" ), tools::translate( "E_FuncLimaType", "HL" ), "LR" ) );
        // $$$$ SBO 2008-08-11: should be "LD"
        lines.push_back( PhaseLineType( eLimaFuncLDM, tools::translate( "E_FuncLimaType", "Start of mission line (SML)" ), tools::translate( "E_FuncLimaType", "SML" ), "LDM" ) );
        // $$$$ SBO 2008-08-11: should not exist
        lines.push_back( PhaseLineType( eLimaFuncLFM, tools::translate( "E_FuncLimaType", "End of mission line (EML)" ), tools::translate( "E_FuncLimaType", "EML" ), "LFM" ) );
        // $$$$ SBO 2008-08-11: LIA
        lines.push_back( PhaseLineType( eLimaFuncLIA, tools::translate( "E_FuncLimaType", "Recognition and reception line (RRL)" ), tools::translate( "E_FuncLimaType", "RRL" ), "LIA" ) );
        return lines;
    }

    const std::vector< PhaseLineType >& PhaseLines()
    {
        static const std::vector< PhaseLineType > lines = InitializePhaseLines();
        return lines;
    }
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_FuncLimaType nType )
{
    if( nType >= 0 && nType < eLimaFuncNbr )
        return PhaseLines()[ nType ].fullName_;
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::ToShortString
// Created: SBO 2008-08-11
// -----------------------------------------------------------------------------
QString tools::ToShortString( E_FuncLimaType nType )
{
    if( nType >= 0 && nType < eLimaFuncNbr )
        return PhaseLines()[ nType ].shortName_;
    return Unknown();
}

// -----------------------------------------------------------------------------
// Name: tools::LimaTypeFromShortString
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
E_FuncLimaType tools::LimaTypeFromShortString( const QString& type )
{
    for( int i = 0; i < int( eLimaFuncNbr ); ++i )
        if( PhaseLines()[i].shortName_ == type )
            return (E_FuncLimaType)i;
    return (E_FuncLimaType)-1;
}

// -----------------------------------------------------------------------------
// Name: tools::LimaTypeFromXmlString
// Created: SBO 2008-08-11
// -----------------------------------------------------------------------------
E_FuncLimaType tools::LimaTypeFromXmlString( const QString& type )
{
    for( int i = 0; i < int( eLimaFuncNbr ); ++i )
        if( PhaseLines()[i].xmlName_ == type )
            return (E_FuncLimaType)i;
    return (E_FuncLimaType)-1;
}

// -----------------------------------------------------------------------------
// Name: tools::LimaTypeShortToXmlString
// Created: SBO 2008-08-11
// -----------------------------------------------------------------------------
QString tools::LimaTypeShortToXmlString( const QString& shortType )
{
    for( int i = 0; i < int( eLimaFuncNbr ); ++i )
        if( PhaseLines()[i].shortName_ == shortType )
            return PhaseLines()[i].xmlName_;
    return "";
}
