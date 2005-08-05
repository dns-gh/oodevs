//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Types.h $
// $Author: Nld $
// $Modtime: 11/05/05 9:58 $
// $Revision: 84 $
// $Workfile: MOS_Types.h $
//
//*****************************************************************************

#ifndef __MOS_Types_h_
#define __MOS_Types_h_

#include "MT_Tools/MT_Assert.h"
//#include "MT_Tools/MT_Scipio_enum.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Rect.h"

#include <qgl.h>

class MOS_Node;
class MOS_Tri;
class MOS_Line;
class MOS_Object;

//-------------------------------------------------------------------------
/** @name Debug mode*/
//-------------------------------------------------------------------------
//@{
#ifdef _DEBUG
#   ifndef MOS_DEBUG
#       define MOS_DEBUG
#   endif 
#endif 
//@}

//-----------------------------------------------------------------------------
// Machine type: simple client-server structure
//-----------------------------------------------------------------------------
enum SCP_MachineType
{
    eAgentServer,
    eMOSServer
};

enum SCP_ConnMagicCookie
{
    eConnMagicMOSServerAgentServer = 1
};

enum E_TypeArme
{
    eTypeArmeUnknown,  
};

enum E_TypeCamp
{
    eTypeCampAmi = 0,
    eTypeCampEnnemi,  
    eTypeCampNeutre,  
    eTypeCampInconnu, 
};


typedef uint E_MissionID;


enum E_NatureAtlasType
{
    eNatureAtlas_Blinde         = 0x8000, // NB : valeurs imposés par l'ASN
    eNatureAtlas_ASS            = 0x4000,
    eNatureAtlas_PC             = 0x2000,
    eNatureAtlas_Log            = 0x1000,
    eNatureAtlas_Mortier        = 0x0800,
    eNatureAtlas_LanceRoquette  = 0x0400,
    eNatureAtlas_Vehicule       = 0x0200,
    eNatureAtlas_PersonnelAPied = 0x0100,
    eNatureAtlas_Helicoptere    = 0x0080,
    eNatureAtlas_Indefini       = 0x0040,
    eNatureAtlas_None           = 0x0020
};


#define MOS_DEFAULT_PORT_AS_MOS 10000

typedef uint32  MIL_AgentID;
typedef uint32  MIL_MOSContextID;
typedef uint32  MIL_SensorCapacityID;
typedef char    MOS_Position[15];

typedef uint32  MIL_LimaID;
typedef uint32  MIL_LimitID;
typedef uint8   MIL_LimaFunctionID;

#define MIL_NULL_MOS_AGENT_ID   (MIL_AgentID)-1
#define MIL_NULL_LIMIT_ID       (MIL_LimitID)-1

#define MT_PI 3.14159265358979323 //$$$ A deplacer


typedef std::vector< MOS_Node* >           T_NodePtrVector;
typedef const T_NodePtrVector              CT_NodePtrVector;
typedef T_NodePtrVector::iterator          IT_NodePtrVector;
typedef T_NodePtrVector::const_iterator    CIT_NodePtrVector;

typedef std::vector< MOS_Line* >           T_MOSLinePtrVector;
typedef const T_MOSLinePtrVector              CT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::iterator          IT_MOSLinePtrVector;
typedef T_MOSLinePtrVector::const_iterator    CIT_MOSLinePtrVector;


typedef std::vector< MOS_Tri* >            T_TriPtrVector;
typedef const T_TriPtrVector               CT_TriPtrVector;
typedef T_TriPtrVector::iterator           IT_TriPtrVector;
typedef T_TriPtrVector::const_iterator     CIT_TriPtrVector;

typedef std::set< MOS_Object* >             T_ObjectPtrSet;
typedef const T_ObjectPtrSet                    CT_ObjectPtrSet;
typedef T_ObjectPtrSet::iterator                IT_ObjectPtrSet;
typedef T_ObjectPtrSet::const_iterator          CIT_ObjectPtrSet;


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

// NB : for the MOC files
namespace DIN
{
    class DIN_Input;
    class DIN_BufferedMessage;
	class DIN_Engine;
	class DIN_Link;
}

#define MOS_SIZESELECT          8.0

#endif // __MOS_Types_h_
