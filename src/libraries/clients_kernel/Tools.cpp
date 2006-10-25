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
    static const QString unknown = tools::translate( "tools", "Inconnu" );
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
        tools::translate( "E_TroopHealthState", "Opérationnels" ),
        tools::translate( "E_TroopHealthState", "Morts" ),
        tools::translate( "E_TroopHealthState", "Blessés" ),
        tools::translate( "E_TroopHealthState", "Blessés mentalement" ),
        tools::translate( "E_TroopHealthState", "Contaminés" ),
        tools::translate( "E_TroopHealthState", "En traitement" ),
        tools::translate( "E_TroopHealthState", "Utilisés pour maintenance" )
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
        tools::translate( "E_PerceptionResult", "Non vu" ),
        tools::translate( "E_PerceptionResult", "Detection" ),
        tools::translate( "E_PerceptionResult", "Reconnaissance" ),
        tools::translate( "E_PerceptionResult", "Identification" ),
    };
    if( nResult >= 0 && nResult < 4 )
        return perceptions[ nResult ];
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