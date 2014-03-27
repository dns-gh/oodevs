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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include <boost/shared_ptr.hpp>

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_PathPoint;
class MIL_AgentPion;
class MIL_Entity_ABC;
class MIL_FragOrder;
class MIL_Mission_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class PHY_ComposanteTypePion;

namespace sword
{
    class Brain;
}

namespace directia
{
    namespace tools
    {
        namespace binders
        {
            class ScriptRef;
        }
    }
}

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_MiscFunctions
{
public:
    // Communication
    static void Report                   ( DEC_Decision_ABC& caller, int type, const std::string& reportId );
    static void ReportAgentKnowledge     ( DEC_Decision_ABC& caller, int type, const std::string& reportId, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    static void ReportDotationType       ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const PHY_DotationCategory* dotationType );
    static void ReportEquipmentType      ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const PHY_ComposanteTypePion* equipmentType );
    static void ReportFloat              ( DEC_Decision_ABC& caller, int type, const std::string& reportId, float param );
    static void ReportIntInt             ( DEC_Decision_ABC& caller, int type, const std::string& reportId, int param1, int param2 );
    static void ReportAgentKnowledgeInt  ( DEC_Decision_ABC& caller, int type, const std::string& reportId, boost::shared_ptr< DEC_Knowledge_Agent > param1, int param2 );
    static void ReportFloatFloat         ( DEC_Decision_ABC& caller, int type, const std::string& reportId, float param1, float param2 );
    static void ReportId                 ( DEC_Decision_ABC& caller, int type, const std::string& reportId, int id );
    static void ReportObjectKnoweldge    ( DEC_Decision_ABC& caller, int type, const std::string& reportId, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static void ReportPion               ( DEC_Decision_ABC& caller, int type, const std::string& reportId, DEC_Decision_ABC* pion );
    static void ReportPionAutomate       ( DEC_Decision_ABC& caller, int type, const std::string& reportId, DEC_Decision_ABC* pion, DEC_Decision_ABC* automate );
    static void ReportPionPion           ( DEC_Decision_ABC& caller, int type, const std::string& reportId, DEC_Decision_ABC* pion1, DEC_Decision_ABC* pion2 );
    static void ReportPopulationKnowledge( DEC_Decision_ABC& caller, int type, const std::string& reportId, int populationKnowledge );
    static void ReportTirPion            ( DEC_Decision_ABC& caller, int type, const std::string& reportId, int id );
    static void ReportString             ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const std::string& message );
    static void ReportStage              ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const std::string& message );

    static void Trace( const MIL_Entity_ABC& caller, const std::string& message );
    static void Debug( const MIL_Entity_ABC& caller, const std::string& callerType,
            const std::string& message );
    static void DebugDrawPoints( const MIL_Entity_ABC& caller,
            std::vector< boost::shared_ptr< MT_Vector2D > > points );
    static void DebugDrawPoint ( const MIL_Entity_ABC& caller, const MT_Vector2D* pPoint  );
    static std::string                 GetPointXY     ( boost::shared_ptr< MT_Vector2D > point );

    // Reinforcement
    static std::vector<DEC_Decision_ABC*> GetAgentReinforcements( const DEC_Decision_ABC* pAgent );
    static std::vector<DEC_Decision_ABC*> GetReinforcements  ( const MIL_AgentPion& callerAgent );
    static int                            GetAgentReinforcementsNumber( const DEC_Decision_ABC* pAgent );
    static bool Reinforce          ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pTarget );
    static void CancelReinforcement( MIL_AgentPion& callerAgent );

    // Misc
    static void SetCurrentSpeedModificator( MIL_AgentPion& callerAgent, double rFactor );
    static void SetMaxSpeedModificator    ( MIL_AgentPion& callerAgent, double rFactor );
    static double GetMaxSpeedModificator( const DEC_Decision_ABC* agent );
    static unsigned int GetTimeInSeconds();

    // Representations
    static std::vector< boost::shared_ptr< MIL_FragOrder > > GetOrdersCategory ( MIL_Entity_ABC& callerAgent );
    static std::vector< boost::shared_ptr< DEC_PathPoint > > GetPointsCategory ( MIL_Entity_ABC& callerAgent );
    static void RemoveFromOrdersCategory                 ( MIL_Entity_ABC& callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder );
    static void DeleteOrderRepresentation                ( MIL_Entity_ABC& callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder );
    static void RemoveFromPointsCategory                 ( MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_PathPoint > pPoint );

    static void AddEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent );
    static void RemoveEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetEnemyRepresentation( DEC_Decision_ABC* caller );

    //
    static void FillMissionParameters                         ( sword::Brain& brain, directia::tools::binders::ScriptRef& initTaskFunction, const directia::tools::binders::ScriptRef& refMission, boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife );

    static std::string  GetName                               ( DEC_Decision_ABC* pEntity );
    static DEC_Decision_ABC* GetAutomate                      ( DEC_Decision_ABC* pAgent );
    static boost::shared_ptr< MT_Vector2D > GetDirectionEnnemi( boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void CopyDirectionDanger                           ( MT_Vector2D* pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission );
};

#endif // __DEC_MiscFunctions_h_
