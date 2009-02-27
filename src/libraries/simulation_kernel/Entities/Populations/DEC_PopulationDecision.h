// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_PopulationDecision.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationDecision_h_
#define __DEC_PopulationDecision_h_

#include "Decision/DEC_Decision.h"

class MIL_Population;
class MIL_PopulationMission;
class NET_ASN_MsgPopulationUpdate;

// =============================================================================
// @class  DEC_PopulationDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationDecision : public DEC_Decision
{
    MT_COPYNOTALLOWED( DEC_PopulationDecision )

public:
             DEC_PopulationDecision( MIL_Population& population );
             DEC_PopulationDecision();
    virtual ~DEC_PopulationDecision();

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
    void Clean         ();

    void StartMissionBehavior( MIL_PopulationMission& mission );
    void StopMissionBehavior ( MIL_PopulationMission& mission );
    virtual void Reset               ();
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation     () const;
    MT_Float        GetDominationState() const;
    bool            HasStateChanged   () const; // Etat decisionnel
    //@}

    //! @name Notifications
    //@{
    void NotifyDominationStateChanged( MT_Float rValue );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgPopulationUpdate& msg );
    void SendFullState   ( NET_ASN_MsgPopulationUpdate& msg );
    //@}

private:
    //! @name Tools
    //@{
    bool              IsDefaultBehaviorAvailable() const;
    void              StartDefaultBehavior      ();
    void              StopDefaultBehavior       ();

    void              CleanStateAfterCrash      ();
    //@}

    //! @name Helpers
    //@{
    virtual void HandleUpdateDecisionError();
    //@}

private:
    MIL_Population*                      pPopulation_;
    DIA_FunctionCaller< MIL_Population > diaFunctionCaller_;
    DIA_Parameters                       defaultBehaviorParameters_;
    DIA_Parameters                       missionBehaviorParameters_;   

    MT_Float                             rDominationState_;
    MT_Float                             rLastDominationState_;
    bool                                 bStateHasChanged_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Population
    static int nDIANameIdx_;

    static uint nDefaultBehaviorDummyId_;
    static uint nMissionBehaviorDummyId_;
};

#endif // __DEC_PopulationDecision_h_
