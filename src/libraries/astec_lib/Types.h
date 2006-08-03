//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Types.h $
// $Author: Ape $
// $Modtime: 7/01/05 11:19 $
// $Revision: 9 $
// $Workfile: Types.h $
//
//*****************************************************************************

#ifndef __Types_h_
#define __Types_h_

//-----------------------------------------------------------------------------
// Machine type: simple client-server structure
//-----------------------------------------------------------------------------

enum SCP_ConnMagicCookie
{
    eConnMagicMOSServerAgentServer = 1
};


enum E_TroopHealthState
{
    eTroopHealthStateTotal = 0,
    eTroopHealthStateOperational,
    eTroopHealthStateDead,
    eTroopHealthStateWounded,
    eTroopHealthStateMentalWounds,
    eTroopHealthStateContaminated,
    eTroopHealthStateInTreatment,
    eTroopHealthStateUsedForMaintenance,
    eTroopHealthStateNbrStates
};

enum E_WeatherType
{
    eWeatherTypeNone,
    eWeatherTypeRainOrSnowStorm,
    eWeatherTypeFog,
    eWeatherTypeDrizzle,
    eWeatherTypeRain,
    eWeatherTypeSnow,
    eWeatherTypeSmoke,

    eNbrWeatherType
};

enum E_LightingType
{
    eLightingJourSansNuage,
    eLightingJourPeuNuageux,
    eLightingJourMoyennementNuageux,
    eLightingJourAssezNuageux,
    eLightingJourTresNuageux,

    eLightingNuitPleineLune,
    eLightingNuitTroisQuartDeLune,
    eLightingNuitDemiLune,
    eLightingNuitQuartDeLune,
    eLightingNuitNouvelleLune,

    eLightingEclairant,

    eNbrLightingType
};

enum E_TypeAgent
{
    eAgent,
    ePopulation
};

#define DEFAULT_PORT_AS_MOS 10000

typedef unsigned long  MIL_AgentID;
typedef unsigned long  MIL_MOSContextID;
typedef unsigned long  MIL_SensorCapacityID;
typedef unsigned long  MIL_LineID;

#define MIL_NULL_AGENT_ID   (MIL_AgentID)-1
#define MIL_NULL_LINE_ID       (MIL_LineID)-1


#endif // __Types_h_
