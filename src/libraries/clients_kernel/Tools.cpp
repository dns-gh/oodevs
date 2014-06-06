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
#include "CommandPostAttributes_ABC.h"
#include "Entity_ABC.h"
#include "ENT/ENT_Tr.h"
#pragma warning( push, 0 )
#pragma warning( disable : 4127 )
#include <QtGui/qapplication.h>
#include <QtCore/qlocale.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtranslator.h>
#include <QtCore/qtextcodec.h>
#pragma warning( pop )
#include <boost/date_time.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: tools::IsoStringToQTime
// Created: ABR 2012-07-19
// -----------------------------------------------------------------------------
QDateTime tools::IsoStringToQDateTime( const std::string& text )
{
    return QDateTime::fromString( QString::fromStdString( text ), "yyyyMMddThhmmss" );
}

// -----------------------------------------------------------------------------
// Name: tools::GDHStringToQTime
// Created: JSR 2013-02-06
// -----------------------------------------------------------------------------
QDateTime tools::GDHStringToQDateTime( const std::string& text )
{
    QString extended = QString::fromStdString( text );
    extended.insert( 13, ':' ); extended.insert( 11, ':' );
    extended.insert(  6, '-' ); extended.insert(  4, '-' );
    return QDateTime::fromString( extended, Qt::ISODate );
}

// -----------------------------------------------------------------------------
// Name: tools::QDateTimeToGDHString
// Created: JSR 2013-11-06
// -----------------------------------------------------------------------------
std::string tools::QDateTimeToGDHString( const QDateTime& dateTime )
{
    QString str = dateTime.toString( Qt::ISODate );
    str.remove( ':' );
    str.remove( '-' );
    return str.toStdString();
}

// -----------------------------------------------------------------------------
// Name: tools::BoostTimeToQTime
// Created: ABR 2012-07-19
// -----------------------------------------------------------------------------
QDateTime tools::BoostTimeToQTime( const boost::posix_time::ptime& btime )
{
    return IsoStringToQDateTime( boost::posix_time::to_iso_string( btime ) );
}

// -----------------------------------------------------------------------------
// Name: tools::QTimeToBoostTime
// Created: ABR 2012-07-19
// -----------------------------------------------------------------------------
boost::posix_time::ptime tools::QTimeToBoostTime( const QDateTime& qtime )
{
    return boost::posix_time::from_iso_string( qtime.toString( "yyyyMMddThhmmss" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: tools::IsTimeLeftToRight
// Created: JSR 2014-02-25
// -----------------------------------------------------------------------------
bool tools::IsTimeLeftToRight()
{
    return !QLocale().timeFormat().endsWith( 'h', Qt::CaseInsensitive );
}

// -----------------------------------------------------------------------------
// Name: tools::BuildDurationString
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
QString tools::BuildDurationString( const QString& hours, const QString& minutes, const QString& seconds )
{
    return IsTimeLeftToRight()
        ? hours + ":" + minutes + ":" + seconds
        : seconds + ":" + minutes + ":" + hours;
}

namespace
{
    QString Fill( int number )
    {
        return ( number  < 10 ? "0" : "" ) + QString::number( number );
    }
}

// -----------------------------------------------------------------------------
// Name: tools::DurationFromSeconds
// Created: JSR 2014-01-27
// -----------------------------------------------------------------------------
QString tools::DurationFromSeconds( int value )
{
    const int minutes = value / 60 % 60;
    const int hours = value / 3600;
    const int seconds = value - hours * 3600 - minutes * 60;
    return BuildDurationString( QString::number( hours ), Fill( minutes ), Fill( seconds ) );
}

// -----------------------------------------------------------------------------
// Name: tools::AddTranslator
// Created: ABR 2012-07-11
// -----------------------------------------------------------------------------
QTranslator* tools::AddTranslator( QApplication& application, const std::string& languageCode, const char* t )
{
    std::unique_ptr< QTranslator > trans( new QTranslator( &application ) );
    const QString file = QString( "%1_%2" ).arg( t ).arg( languageCode.c_str() );
    if( trans->load( file, "." ) || trans->load( file, "resources/locales" ) )
    {
        application.installTranslator( trans.get() );
        return trans.release();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: tools::translate
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
QString tools::translate( const char* context, const char* what )
{
    if( qApp )
        return qApp->translate( context, what );
    return QString::fromUtf8( what );
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
    return QString::fromStdString( ENT_Tr::ConvertFromLightingType( lighting, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToDisplayedString
// Created: JSR 2010-07-09
// -----------------------------------------------------------------------------
QString tools::ToDisplayedString( E_WeatherType weather )
{
    return QString::fromStdString( ENT_Tr::ConvertFromWeatherType( weather, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_PopulationAttitude nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromPopulationAttitude( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_ObstacleActivation nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromObstacleActivation( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitPosture nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromUnitPosture( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_OperationalStatus nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromOperationalStatus( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_ForceRatioStatus nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromForceRatioStatus( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_Roe nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromRoe( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_PopulationRoe nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromPopulationRoe( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_MeetingEngagementStatus nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromMeetingEngagementStatus( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_FireAvailability nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromFireAvailability( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_NatureLevel nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */  )
{
    return QString::fromStdString( ENT_Tr::ConvertFromNatureLevel( nType, conversion ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitExperience nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromUnitExperience( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitMorale nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromUnitMorale( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitStress nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromUnitStress( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_UnitTiredness nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromUnitTiredness( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2007-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanWound nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromHumanWound( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: AGE 2008-06-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_LocationType nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromLocationType( nType ) );
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
QString tools::ToString( E_AgentNbcSuit nType )
{
    return QString::fromStdString( ENT_Tr::ConvertFromAgentNbcSuit( nType, ENT_Tr::eToTr ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanRank nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */ )
{
    return QString::fromStdString( ENT_Tr::ConvertFromHumanRank( nType, conversion ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanState nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */ )
{
    return QString::fromStdString( ENT_Tr::ConvertFromHumanState( nType, conversion ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
QString tools::ToString( E_HumanLocation nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */ )
{
    return QString::fromStdString( ENT_Tr::ConvertFromHumanLocation( nType, conversion ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_EquipmentState nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */ )
{
    return QString::fromStdString( ENT_Tr::ConvertFromEquipmentState( nType, conversion ) );
}

// -----------------------------------------------------------------------------
// Name: tools::ToString
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
QString tools::ToString( E_InjuriesSeriousness nType, ENT_Tr::E_Conversion conversion /* = ENT_Tr::eToTr */ )
{
    return QString::fromStdString( ENT_Tr::ConvertFromInjuriesSeriousness( nType, conversion ) );
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
