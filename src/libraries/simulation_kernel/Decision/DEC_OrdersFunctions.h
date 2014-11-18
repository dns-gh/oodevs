// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_OrdersFunctions.h $
// $Author: Nld $
// $Modtime: 2/12/04 10:15 $
// $Revision: 3 $
// $Workfile: DEC_OrdersFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_OrdersFunctions_h_
#define __DEC_OrdersFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Mission_ABC;
class MIL_AutomateMission;
class MIL_Fuseau;
class MT_Vector2D;
class MIL_LimaOrder;

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_OrdersFunctions
{
public:
    static void Register( sword::Brain& brain );

    // Mission
    static void FinishMission( DEC_Decision_ABC* caller );
    static bool IsNewMissionStarted( DEC_Decision_ABC* caller );

    // Limas
    static unsigned int GetLima( const DEC_Decision_ABC* caller, unsigned int limaId );
    static std::vector< unsigned int > GetLimasFromType ( const DEC_Decision_ABC* caller, unsigned int limaId );
    static MIL_LimaOrder* GetNextScheduledLima( const DEC_Decision_ABC* caller );
    static const MIL_Fuseau& GetFuseau            ( const DEC_Decision_ABC& caller );
    static bool GetMissionLimaFlag( const DEC_Decision_ABC* caller, unsigned int limaId );
    static void PionSetMissionLimaFlag            ( MIL_AgentPion& caller, unsigned int limaId, bool flag );
    static void PionSetMissionLimaScheduleFlag    ( MIL_AgentPion& caller, unsigned int limaId, bool flag  );
    static void AutomateSetMissionLimaFlag        ( DEC_Decision_ABC* caller, unsigned int limaId, bool flag  );
    static void AutomateSetMissionLimaScheduleFlag( DEC_Decision_ABC* caller, unsigned int limaId, bool flag  );

    // Automate
    static boost::shared_ptr< MIL_Mission_ABC > MRT_CreatePionMission  ( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission );
    static void             MRT_Validate           ( DEC_Decision_ABC* callerAutomate );
    static void             MRT_AffectFuseaux      ( DEC_Decision_ABC* callerAutomate, std::vector< DEC_Decision_ABC* > pions );
    static boost::shared_ptr< MIL_Mission_ABC > CDT_CreatePionMission  ( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pPion, const std::string& mission );
    static boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionBM    ( DEC_Decision_ABC* pPion, const std::string& mission );
    static boost::shared_ptr< MIL_Mission_ABC > CreatePionMissionVersPionBM    ( DEC_Decision_ABC* pPion, const std::string& mission );
    static void             CDT_GivePionMission    ( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GivePionMissionVersPion    ( MIL_Automate& callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GiveMission    ( DEC_Decision_ABC* callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void             CDT_GiveMissionVersPion    ( DEC_Decision_ABC* callerPion, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static boost::shared_ptr< MIL_Mission_ABC > CreateAutomateMission  ( DEC_Decision_ABC* callerAutomate, DEC_Decision_ABC* pAutomate, const std::string& mission  );
    static void AssignFuseauToPionMission       ( MIL_Fuseau* pFuseau, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void AssignFuseauToAutomateMission   ( MIL_Fuseau* pFuseau, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void AssignDirectionToAutomateMission( MT_Vector2D* pDirection, boost::shared_ptr< MIL_Mission_ABC > pMission);
    static void GiveAutomateMission             ( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void GiveAutomateMissionToAutomat    ( DEC_Decision_ABC* callerAutomate, boost::shared_ptr< MIL_Mission_ABC > pMission );
    static void GiveMissionToAutomat            ( boost::shared_ptr< MIL_Mission_ABC > pMission );
    static std::list<MIL_Fuseau*> SplitFuseau   ( const DEC_Decision_ABC* callerAutomate, unsigned int nbrSubFuseaux );

    static bool IsPionMissionAvailable( DEC_Decision_ABC* agent, std::string diaType );
    static bool IsAutomateMissionAvailable( DEC_Decision_ABC* agent, std::string diaType );
    static bool IsFragOrderAvailable( DEC_Decision_ABC* agent, const std::string& fragorder );

    static bool DEC_Mission_IsPath( boost::shared_ptr< MIL_Mission_ABC > pMission, const std::string& parameter );

private:
    //! @name Tools
    //@{
    static MIL_Automate& GetHigherEngagedAutomate( MIL_Automate& automate );
    //@}
};

#endif // __DEC_OrdersFunctions_h_
