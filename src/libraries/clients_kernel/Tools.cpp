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

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
const char* tools::ToString( E_UnitPosture nPosture )
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
// Name: tools::ToString
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
const char* tools::ToString( E_TroopHealthState nState )
{
    switch( nState )
    {
        case eTroopHealthStateTotal:              return "Total";
        case eTroopHealthStateOperational:        return "Opérationnels";
        case eTroopHealthStateDead:               return "Morts";
        case eTroopHealthStateWounded:            return "Blessés";
        case eTroopHealthStateMentalWounds:       return "Blessés mentalement";
        case eTroopHealthStateContaminated:       return "Contaminés";
        case eTroopHealthStateInTreatment:        return "En traitement";
        case eTroopHealthStateUsedForMaintenance: return "Utilisés pour maintenance";
        default:
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
const char* tools::ToString( E_PerceptionResult nResult )
{
    switch( nResult )
    {
        case eIdentification:  return "Identification";
        case eRecognition:     return "Reconnaissance";
        case eDetection:       return "Detection";
        case eNotSeen:         return "Non vu";
        default:
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
const char* tools::ToString( E_WeatherType weather )
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
// Name: tools::ToString
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
const char* tools::ToString( E_LightingType weather )
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