// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 10 $
// $Workfile: DEC_RolePion_Decision.h $
//
// *****************************************************************************

#ifndef __DEC_RolePion_Decision_h_
#define __DEC_RolePion_Decision_h_

#include "MIL.h"
#include "Decision/DEC_Decision_ABC.h"

class MIL_AgentPion;
class MIL_PionMission;
class NET_ASN_MsgUnitAttributes;
class DEC_Path;
class PHY_RoePopulation;

enum E_FightRateState;
enum E_RulesOfEngagementState;
enum E_CloseCombatState;
enum E_FireAvailability;

// =============================================================================
// @class  DEC_RolePion_Decision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_RolePion_Decision : public DEC_Decision_ABC
                            , public DIA_Engine
{
    MT_COPYNOTALLOWED( DEC_RolePion_Decision )

public:
             DEC_RolePion_Decision( MT_RoleContainer& role, MIL_AgentPion& pion );
             DEC_RolePion_Decision();
    virtual ~DEC_RolePion_Decision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    
    //! @name Init
    //@{
    static void InitializeDIA();
    //@}
    
    //! @name Operations
    //@{
    void UpdateDecision();
    void Clean         ();

    void StartMissionBehavior( MIL_PionMission& mission );
    void StopMissionBehavior ( MIL_PionMission& mission );
    void Reset               ();

    void NotifyAutomateChanged();
    //@}

    //! @name Accessors
    //@{
          MIL_AgentPion&      GetPion         () const;
          DIA_BehaviorPart&   GetBehaviorPart () const;
          DIA_Knowledge_Part& GetKnowledgePart() const;
          bool                HasStateChanged () const; // Etat decisionnel
    const PHY_RoePopulation&  GetRoePopulation() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

    //! @name Notifications
    //@{
    void NotifyForceRatioStateChanged         ( E_ForceRatioState        nState );
    void NotifyRulesOfEngagementStateChanged  ( E_RulesOfEngagementState nState );
    void NotifyCloseCombatStateChanged        ( E_CloseCombatState       nState );
    void NotifyOperationalStateChanged        ( E_OperationalState       nState );
    void NotifyIndirectFireAvailabilityChanged( E_FireAvailability       nState );
    void NotifyRoePopulationChanged           ( const PHY_RoePopulation& roe );
    //@}

private:
    //! @name Tools
    //@{
    bool IsDefaultBehaviorAvailable() const;
    void StartDefaultBehavior      ();
    void StopDefaultBehavior       ();

    void CleanStateAfterCrash      ();
    //@}

private:
    MIL_AgentPion*                      pPion_;
    DIA_FunctionCaller< MIL_AgentPion > diaFunctionCaller_;
    DIA_Parameters                      defaultBehaviorParameters_;
    DIA_Parameters                      missionBehaviorParameters_;   

    // Etat décisionnel
          E_ForceRatioState        nForceRatioState_;
          E_RulesOfEngagementState nRulesOfEngagementState_;
          E_CloseCombatState       nCloseCombatState_;
          E_OperationalState       nOperationalState_;
          E_FireAvailability       nIndirectFireAvailability_;
    const PHY_RoePopulation*       pRoePopulation_; //$$$ à déplacer dans Role_Population ?

    // Network
    bool bStateHasChanged_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Pion
    static int nDIANameIdx_;
    static int nDIAAutomateIdx_;

    static uint nDefaultBehaviorDummyId_;
    static uint nMissionBehaviorDummyId_;
};

#include "DEC_RolePion_Decision.inl"

#endif // __DEC_RolePion_Decision_h_
