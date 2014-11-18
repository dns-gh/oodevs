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
class TER_PathPoint;
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
    static void Register( sword::Brain& brain, bool isMasalife );

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
    static void ReportPionInt            ( DEC_Decision_ABC& caller, int type, const std::string& reportId, DEC_Decision_ABC* param1, int param2 );
    static void ReportPopulationKnowledge( DEC_Decision_ABC& caller, int type, const std::string& reportId, int populationKnowledge );
    static void ReportTirPion            ( DEC_Decision_ABC& caller, int type, const std::string& reportId, int id );
    static void ReportString             ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const std::string& message );
    static void ReportStage              ( DEC_Decision_ABC& caller, int type, const std::string& reportId, const std::string& message );

    static void Trace( const DEC_Decision_ABC* caller, const std::string& message );
    static void Debug( const DEC_Decision_ABC* caller, const std::string& message );
    static void DebugDrawPoints( const DEC_Decision_ABC* caller,
            std::vector< boost::shared_ptr< MT_Vector2D > > points );
    static void DebugDrawPoint ( const DEC_Decision_ABC* caller, const MT_Vector2D* pPoint  );
    static std::string                 GetPointXY     ( boost::shared_ptr< MT_Vector2D > point );

    // Reinforcement
    static std::vector<DEC_Decision_ABC*> GetAgentReinforcements( const DEC_Decision_ABC* pAgent );
    static std::vector<DEC_Decision_ABC*> GetReinforcements  ( const DEC_Decision_ABC* callerAgent );
    static int                            GetAgentReinforcementsNumber( const DEC_Decision_ABC* pAgent );
    static bool Reinforce          ( DEC_Decision_ABC*, const DEC_Decision_ABC* pTarget );
    static void CancelReinforcement( DEC_Decision_ABC* callerAgent );

    // Misc
    static void SetCurrentSpeedModificator( DEC_Decision_ABC* callerAgent, double rFactor );
    static void SetMaxSpeedModificator    ( DEC_Decision_ABC* callerAgent, double rFactor );
    static double GetMaxSpeedModificator( const DEC_Decision_ABC* agent );
    static unsigned int GetTimeInSeconds();

    // Representations
    static std::vector< boost::shared_ptr< MIL_FragOrder > > GetOrdersCategory ( const DEC_Decision_ABC* callerAgent );
    static std::vector< boost::shared_ptr< TER_PathPoint > > GetPointsCategory ( const DEC_Decision_ABC* callerAgent );
    static void RemoveFromOrdersCategory                 ( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder );
    static void DeleteOrderRepresentation                ( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< MIL_FragOrder > pOrder );
    static void RemoveFromPointsCategory                 ( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< TER_PathPoint > pPoint );

    static void AddEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent );
    static void RemoveEnemyRepresentation( const boost::shared_ptr< DEC_Knowledge_Agent >& agent );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetEnemyRepresentation( DEC_Decision_ABC* caller );

    //
    static void FillMissionParameters                         ( sword::Brain& brain, directia::tools::binders::ScriptRef& initTaskFunction, const directia::tools::binders::ScriptRef& refMission, boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife );

    static std::string  GetName                               ( DEC_Decision_ABC* pEntity );
    static DEC_Decision_ABC* GetAutomate                      ( DEC_Decision_ABC* pAgent );
    static void CopyDirectionDanger                           ( MT_Vector2D* pPosSource, boost::shared_ptr< MIL_Mission_ABC > pMission );
};

#endif // __DEC_MiscFunctions_h_
