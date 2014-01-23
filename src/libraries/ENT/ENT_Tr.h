// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ENT_Tr_h_
#define __ENT_Tr_h_

#include "ENT_Enums.h"
#include <string>

namespace sword
{
    enum MagicAction_Type;
    enum UnitMagicAction_Type;
    enum KnowledgeMagicAction_Type;
    enum ObjectMagicAction_Type;
    enum EnumLogisticLevel;
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
}

#define DECLARE_CONVERT_METHODS( NAME, DEFAULT_FROM, DEFAULT_TO )                       \
const std::string& ConvertFrom ## NAME ## ( E_ ## NAME value,                           \
                                            E_Conversion conversion = DEFAULT_FROM );   \
E_ ## NAME ConvertTo ## NAME ##( const std::string& text,                               \
                                 E_Conversion conversion = DEFAULT_TO );                \

#define DECLARE_CONVERT_METHODS_PROTO( CLASS, ALIAS, DEFAULT_FROM, DEFAULT_TO )         \
const std::string& ConvertFrom ## ALIAS ## ( sword:: ## CLASS value,                    \
                                             E_Conversion conversion = DEFAULT_FROM );  \
sword:: ## CLASS ConvertTo ## ALIAS ##( const std::string& text,                        \
                                        E_Conversion conversion = DEFAULT_TO );         \

#define DECLARE_CONVERT_METHODS_SUB_PROTO( CLASS, NAME, ALIAS, DEFAULT_FROM, DEFAULT_TO )       \
const std::string& ConvertFrom ## ALIAS ## ( sword:: ## CLASS ## _ ## NAME value,               \
                                             E_Conversion conversion = DEFAULT_FROM );          \
sword:: ## CLASS ## _ ## NAME ConvertTo ## ALIAS ##( const std::string& text,                   \
                                                     E_Conversion conversion = DEFAULT_TO );    \

namespace ENT_Tr
{
    enum E_Conversion
    {
        eToSim,
        eToApp,
        eToTr
    };

    void InitTranslations();

    DECLARE_CONVERT_METHODS( AgentNbcSuit, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( AmmunitionType, eToSim, eToSim )
    DECLARE_CONVERT_METHODS( AviationRange, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( BreakdownNTI, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( BreakdownType, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( CrossingType, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( Diplomacy, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( DotationFamily, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( EquipmentState, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( EventDockModes, eToTr, eToTr );
    DECLARE_CONVERT_METHODS( EventTypes, eToTr, eToTr );
    DECLARE_CONVERT_METHODS( FireAvailability, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( ForceRatioStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( GhostType, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( HumanLocation, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( HumanRank, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( HumanState, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( HumanWound, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( InjuriesSeriousness, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LayerTypes, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LightingType, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( Location, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LocationType, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LogFuneralHandlingStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LogMedicalHandlingStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( LogSupplyHandlingStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( MeetingEngagementStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( MissionType, eToTr, eToTr );
    DECLARE_CONVERT_METHODS( Modes, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( NatureLevel, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( NbcState, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( ObstacleActivation, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( OperationalStatus, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( PopulationAttitude, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( PopulationRoe, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( Roe, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( UnitExperience, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( UnitMorale, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( UnitPosture, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( UnitStress, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( UnitTiredness, eToSim, eToSim );
    DECLARE_CONVERT_METHODS( WeatherType, eToSim, eToSim );

    DECLARE_CONVERT_METHODS_PROTO( EnumLogisticLevel, LogisticLevel, eToTr, eToSim );
    DECLARE_CONVERT_METHODS_SUB_PROTO( KnowledgeMagicAction, Type, KnowledgeMagicActionType, eToTr, eToSim );
    DECLARE_CONVERT_METHODS_SUB_PROTO( LogMaintenanceHandlingUpdate, EnumLogMaintenanceHandlingStatus, LogMaintenanceHandlingStatus, eToTr, eToSim );
    DECLARE_CONVERT_METHODS_SUB_PROTO( MagicAction, Type, MagicActionType, eToTr, eToSim );
    DECLARE_CONVERT_METHODS_SUB_PROTO( ObjectMagicAction, Type, ObjectMagicActionType, eToTr, eToSim );
    DECLARE_CONVERT_METHODS_SUB_PROTO( UnitMagicAction, Type, UnitMagicActionType, eToTr, eToSim );
}

#endif // __ENT_Tr_h_
