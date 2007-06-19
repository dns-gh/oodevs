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
#include <qstring.h>

// =============================================================================
/** @class  Tools
    @brief  Crappy translation tools  
*/
// Created: AGE 2006-03-15
// =============================================================================
namespace tools
{
    QString translate( const char* context, const char* what );

    QString ToString( kernel::E_TroopHealthState nState );
    QString ToString( kernel::E_PerceptionResult nResult );
    QString ToString( kernel::E_LightingType lighting );
    QString ToString( kernel::E_WeatherType weather );

    QString ToString( E_PopulationAttitude nType );
    QString ToString( E_TypeObstacle nType );
    QString ToString( E_UnitPosture nType );
    QString ToString( E_EtatOperationnel nType );
    QString ToString( E_EtatRapFor nType );
    QString ToString( E_Roe nType );
    QString ToString( E_RoePopulation nType );
    QString ToString( E_EtatCombatRencontre nType );
    QString ToString( E_DisponibiliteAuTir nType );
    QString ToString( E_NatureLevel nType );
    QString ToString( E_UnitExperience nType );
    QString ToString( E_UnitMoral nType );
    QString ToString( E_UnitFatigue nType );
    QString ToString( E_HumanWound nType );
    QString ToString( E_LogRavitaillementTraitementEtat nType );
    QString ToString( E_LogSanteTraitementEtat nType );
    QString ToString( E_LogMaintenanceTraitementEtat nType );

    QString Unknown();

    const char* GetXmlSection( E_UnitPosture nPosture );
    const char* GetXmlSection( kernel::E_WeatherType nWeatherType );
    const char* GetXmlSection( kernel::E_LightingType nLightingType );

    E_ObjectType       ObjectTypeFromString     ( const std::string& type );
    E_FamilleDotation  FamilleDotationFromString( const std::string& type );
    E_NatureLevel      NatureLevelFromString    ( const std::string& type );
};

#endif // __Kernel_Tools_h_
