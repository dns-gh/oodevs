// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_MiscFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 16:18 $
// $Revision: 4 $
// $Workfile: DEC_MiscFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_MiscFunctions_h_
#define __DEC_MiscFunctions_h_

#include "MIL.h"
#include "MT_Tools/MT_Random.h"
#include <boost/shared_ptr.hpp>

#include "simulation_orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "game_asn/ASN_Delete.h"

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class DEC_PathPoint;
class MIL_AgentPion;
class MIL_Entity_ABC;
class MIL_FragOrder;
class MIL_Mission_ABC;
class MT_Vector2D;

namespace directia
{
    class ScriptRef;
}

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_MiscFunctions
{
public:
    // Communication
    template< typename T > static void Report                   ( T& caller, int type, int reportId );
    template< typename T > static void ReportAgentKnowledge     ( T& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    template< typename T > static void ReportDotationType       ( T& caller, int type, int reportId, const PHY_DotationCategory* dotationType );
    template< typename T > static void ReportEquipmentType      ( T& caller, int type, int reportId, const PHY_ComposanteTypePion* equipmentType );
    template< typename T > static void ReportFloat              ( T& caller, int type, int reportId, float param );
    template< typename T > static void ReportFloatFloat         ( T& caller, int type, int reportId, float param1, float param2 );
    template< typename T > static void ReportId                 ( T& caller, int type, int reportId, int id );
    template< typename T > static void ReportObjectKnoweldge    ( T& caller, int type, int reportId, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static void ReportPion               ( T& caller, int type, int reportId, DEC_Decision_ABC* pion );
    template< typename T > static void ReportPionAutomate       ( T& caller, int type, int reportId, DEC_Decision_ABC* pion, DEC_Decision_ABC* automate );
    template< typename T > static void ReportPopulationKnowledge( T& caller, int type, int reportId, int populationKnowledge );
    template< typename T > static void ReportTirPion            ( T& caller, int type, int reportId, int id );

    template< typename T > static void Trace          			( const T& caller, const std::string& message );
    template< typename T > static void Debug          			( const T& caller, const std::string& callerType, const std::string& message );
    template< typename T > static void DebugDrawPoints			( const T& caller, std::vector< boost::shared_ptr< MT_Vector2D > > points );
    template< typename T > static void DebugDrawPoint 			( const T& caller, const MT_Vector2D* pPoint  );
  
    // Reinforcement
    static std::vector<DEC_Decision_ABC*> GetReinforcements  ( const MIL_AgentPion& callerAgent );
    static bool Reinforce          ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget );            
    static void CancelReinforcement( MIL_AgentPion& callerAgent );           

    // Misc
    static void SetCurrentSpeedModificator( MIL_AgentPion& callerAgent, MT_Float rFactor );
    static void SetMaxSpeedModificator    ( MIL_AgentPion& callerAgent, MT_Float rFactor );
    
    // Representations
    static std::vector<MIL_FragOrder*> GetOrdersCategory ( MIL_Entity_ABC& callerAgent );
    static std::vector<DEC_PathPoint*> GetPointsCategory ( MIL_Entity_ABC& callerAgent );
    static void RemoveFromOrdersCategory                 ( MIL_Entity_ABC& callerAgent, MIL_FragOrder* pOrder );
    static void DeleteOrderRepresentation                ( MIL_Entity_ABC& callerAgent, MIL_FragOrder* pOrder );
    static void RemoveFromPointsCategory                 ( MIL_Entity_ABC& callerAgent, DEC_PathPoint* pPoint );
    
    //
    static void FillMissionParameters                         ( directia::ScriptRef& initTaskFunction, const directia::ScriptRef& refMission, MIL_Mission_ABC* mission );
    
    static std::string  GetName                               ( DEC_Decision_ABC* pEntity );
    static DEC_Decision_ABC* GetAutomate                      ( DEC_Decision_ABC* pAgent );
    static boost::shared_ptr< MT_Vector2D > GetDirectionEnnemi( MIL_Mission_ABC* pMission );
    static void CopyDirectionDanger                           ( MT_Vector2D* pPosSource, MIL_Mission_ABC* pMission );
};

#include "DEC_MiscFunctions.inl"

#endif // __DEC_MiscFunctions_h_
