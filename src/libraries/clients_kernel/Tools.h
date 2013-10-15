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
#include "ENT/ENT_Tr_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>
#pragma warning( pop )
#include <boost/date_time/posix_time/ptime.hpp>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  Tools
    @brief  Crappy translation tools
*/
// Created: AGE 2006-03-15
// =============================================================================
namespace tools
{
    QDateTime IsoStringToQTime( const std::string& text );
    QDateTime GDHStringToQTime( const std::string& text );

    QDateTime BoostTimeToQTime( const boost::posix_time::ptime& btime );
    boost::posix_time::ptime QTimeToBoostTime( const QDateTime& qtime );

    QLocale readLocale();
    std::string readLang();
    QTranslator* AddTranslator( QApplication& application, const QLocale& locale, const char* t );

    QString translate( const char* context, const char* what );
    QString findTranslation( const char* context, const char* what );

    // We should really do something about our enum.
    // I think that an enum base class with the following support would be very helpful:
    //  - iteration (++ and -- operators)
    //  - int cast operator (to avoid each static_cast< E_Something >( i ))
    //  - string conversion (that contain both these conversions and those in ENT)
    QString ToString( kernel::E_TroopHealthState nState );
    QString ToString( kernel::E_PerceptionResult nResult );
    QString ToString( E_WeatherType weather );
    QString ToString( kernel::E_FuncLimaType nType );
    QString ToShortString( kernel::E_FuncLimaType nType );

    QString ToString( E_PopulationAttitude nType );
    QString ToString( E_ObstacleActivation nType );
    QString ToString( E_UnitPosture nType );
    QString ToString( E_OperationalStatus nType );
    QString ToString( E_ForceRatioStatus nType );
    QString ToString( E_Roe nType );
    QString ToString( E_PopulationRoe nType );
    QString ToString( E_MeetingEngagementStatus nType );
    QString ToString( E_FireAvailability nType );
    QString ToString( E_NatureLevel nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );
    QString ToString( E_UnitExperience nType );
    QString ToString( E_UnitMorale nType );
    QString ToString( E_UnitStress nType );
    QString ToString( E_UnitTiredness nType );
    QString ToString( E_HumanWound nType );
    QString ToString( E_LogSupplyHandlingStatus nType );
    QString ToString( E_LogMedicalHandlingStatus nType );
    QString ToString( E_LogMaintenanceHandlingStatus nType );
    QString ToString( E_LogFuneralHandlingStatus nType );
    QString ToString( E_LocationType nType );
    QString ToString( E_AgentNbcSuit nType );
    QString ToString( E_DotationFamily nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToSim );
    QString ToString( E_EquipmentState nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );
    QString ToString( E_HumanRank nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );
    QString ToString( E_HumanState nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );
    QString ToString( E_HumanLocation nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );
    QString ToString( E_InjuriesSeriousness nType, ENT_Tr_ABC::E_Conversion conversion = ENT_Tr_ABC::eToTr );

    QString Unknown();

    QString ToDisplayedString( E_LightingType lighting );
    QString ToDisplayedString( E_WeatherType weather );

    E_DotationFamily        DotationFamilyFromString( const std::string& type );
    E_NatureLevel           NatureLevelFromString   ( const std::string& type );
    E_LocationType          LocationFromString      ( const std::string& type );

    kernel::E_FuncLimaType  LimaTypeFromShortString ( const QString& type );
    kernel::E_FuncLimaType  LimaTypeFromXmlString   ( const QString& type );
    QString                 LimaTypeShortToXmlString( const QString& shortType );

    bool IsCommandPost( const kernel::Entity_ABC& entity );
    bool IsEngaged( const kernel::Entity_ABC& entity );

    bool LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
}

#endif // __Kernel_Tools_h_
