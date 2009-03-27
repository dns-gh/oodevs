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
    if( qApp )
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
        PhaseLineType( kernel::E_FuncLimaType type, const QString& fullName, const QString& shortName, const QString& xmlName )
            : type_( type )
            , fullName_( fullName )
            , shortName_( shortName )
            , xmlName_( xmlName )
        {}
        kernel::E_FuncLimaType type_;
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
QString tools::ToShortString( kernel::E_FuncLimaType nType )
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
kernel::E_FuncLimaType tools::LimaTypeFromXmlString( const QString& type )
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
