// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_AutomateDecision.h $
//
// *****************************************************************************

#ifndef __DEC_AutomateDecision_h_
#define __DEC_AutomateDecision_h_

#include "MIL.h"
#include "Decision/DEC_Decision_ABC.h"

class MIL_Automate;
class MIL_AutomateMission;

class NET_ASN_MsgAutomatAttributes;

enum E_ForceRatioState;
enum E_RulesOfEngagementState;
enum E_CloseCombatState;

// =============================================================================
// @class  DEC_AutomateDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_AutomateDecision : public DEC_Decision_ABC
                           , public DIA_Engine
{
    MT_COPYNOTALLOWED( DEC_AutomateDecision )

public:
    explicit DEC_AutomateDecision( MIL_Automate& automate );
             DEC_AutomateDecision();
    virtual ~DEC_AutomateDecision();

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
    virtual void UpdateDecision();
    virtual void Reset          ();
    void Clean          ();
    bool HasStateChanged() const;

    void StartMissionMrtBehavior     ( MIL_AutomateMission& mission );
    void StopMissionMrtBehavior      ( MIL_AutomateMission& mission );
    void StartMissionConduiteBehavior( MIL_AutomateMission& mission );
    void StopMissionConduiteBehavior ( MIL_AutomateMission& mission );
    //@}

    //! @name Notifications
    //@{
    void NotifyForceRatioStateChanged       ( E_ForceRatioState        nState );
    void NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState );
    void NotifyCloseCombatStateChanged      ( E_CloseCombatState       nState );
    void NotifyOperationalStateChanged      ( E_OperationalState       nState );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgAutomatAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgAutomatAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_Automate& GetAutomate() const;
    //@}

private:
    //! @name Tools
    //@{
    DIA_BehaviorPart& GetBehaviorPart           () const;
    bool              IsDefaultBehaviorAvailable() const;
    void              StartDefaultBehavior      ();
    void              StopDefaultBehavior       ();

    void              CleanStateAfterCrash      ();
    //@}

private:
    MIL_Automate*                      pAutomate_;
    DIA_FunctionCaller< MIL_Automate > diaFunctionCaller_;
    DIA_Parameters                     defaultBehaviorParameters_;
    DIA_Parameters                     missionMrtBehaviorParameters_;   
    DIA_Parameters                     missionConduiteBehaviorParameters_;   

    // Etat décisionnel
    E_ForceRatioState        nForceRatioState_;
    E_RulesOfEngagementState nRulesOfEngagementState_;
    E_CloseCombatState       nCloseCombatState_;
    E_OperationalState       nOperationalState_;
    bool                     bStateHasChanged_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Automate
    //    static int nDIANameIdx_;

    static uint nDefaultBehaviorDummyId_;
    static uint nMissionMrtBehaviorDummyId_;
    static uint nMissionConduiteBehaviorDummyId_;
};

#include "DEC_AutomateDecision.inl"

#endif // __DEC_AutomateDecision_h_
