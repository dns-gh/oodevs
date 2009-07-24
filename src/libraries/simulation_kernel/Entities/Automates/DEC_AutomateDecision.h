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

#include "Decision/DEC_Decision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"

class MIL_Mission_ABC;

class NET_ASN_MsgAutomatAttributes;

enum E_ForceRatioState;
enum E_RulesOfEngagementState;
enum E_CloseCombatState;

// =============================================================================
// @class  DEC_AutomateDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_AutomateDecision : public DEC_Decision< MIL_Automate >
                           , private boost::noncopyable
{

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
    void Clean          ();
    bool HasStateChanged() const;

    void StartMissionMrtBehavior     ( MIL_Mission_ABC& mission );
    void StopMissionMrtBehavior      ( MIL_Mission_ABC& mission );
    void StartMissionConduiteBehavior( MIL_Mission_ABC& mission );
    void StopMissionConduiteBehavior ( MIL_Mission_ABC& mission );
    //@}

    //! @name Notifications
    //@{
    void NotifyCloseCombatStateChanged      ( E_CloseCombatState       nState );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgAutomatAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgAutomatAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_Automate& GetAutomate() const;
    virtual DEC_AutomateDecision* GetDecAutomate() const;
    virtual std::string GetName() const;
    //@}

private:
    //! @name Tools
    //@{
    virtual void      EndCleanStateAfterCrash      ();
    //@}

private:
    DIA_FunctionCaller< MIL_Automate > diaFunctionCaller_;
    DIA_Parameters                     missionMrtBehaviorParameters_;   
    DIA_Parameters                     missionConduiteBehaviorParameters_;   

    // Etat décisionnel
    E_RulesOfEngagementState nRulesOfEngagementState_;
    E_CloseCombatState       nCloseCombatState_;
    E_OperationalState       nOperationalState_;
    bool                     bStateHasChanged_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Automate
    //    static int nDIANameIdx_;
};

#endif // __DEC_AutomateDecision_h_
