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

class MT_XXmlInputArchive;
template< typename Archive > class MT_InputArchive_Logger;
typedef MT_InputArchive_Logger< MT_XXmlInputArchive > InputArchive;

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

typedef uint32  MIL_AgentID;
typedef uint32  MIL_MOSContextID;
typedef uint32  MIL_SensorCapacityID;
typedef uint32  MIL_LineID;

#define MIL_NULL_AGENT_ID   (MIL_AgentID)-1
#define MIL_NULL_LINE_ID       (MIL_LineID)-1

#define MT_PI 3.14159265358979323 //$$$ A deplacer


class Node;
class Tri;
class Line;

typedef std::vector< Node* >           T_NodePtrVector;
typedef const T_NodePtrVector              CT_NodePtrVector;
typedef T_NodePtrVector::iterator          IT_NodePtrVector;
typedef T_NodePtrVector::const_iterator    CIT_NodePtrVector;

typedef std::vector< Line* >           T_MOSLinePtrVector;
typedef const T_MOSLinePtrVector              CT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::iterator          IT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::const_iterator    CIT_MOSLinePtrVector;

typedef std::vector< Tri* >            T_TriPtrVector;
typedef const T_TriPtrVector               CT_TriPtrVector;
typedef T_TriPtrVector::iterator           IT_TriPtrVector;
typedef T_TriPtrVector::const_iterator     CIT_TriPtrVector;


// NB : for the MOC files
namespace DIN
{
    class DIN_Input;
    class DIN_BufferedMessage;
	class DIN_Engine;
	class DIN_Link;
}


enum E_DataFlow
{
    eDefault,
    eSent,
    eReceived
};


#endif // __Types_h_
