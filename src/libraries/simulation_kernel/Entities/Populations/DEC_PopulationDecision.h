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
#include "Entities/Populations/MIL_Population.h"

class MIL_Mission_ABC;
class NET_ASN_MsgPopulationUpdate;

// =============================================================================
// @class  DEC_PopulationDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationDecision : public DEC_Decision< MIL_Population >
                             , private boost::noncopyable
{

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
    
    //! @name Operations
    //@{
    void Clean         ();

    virtual void StartMissionBehavior( MIL_Mission_ABC& mission );
    virtual void StopMissionBehavior ( MIL_Mission_ABC& mission );
    //@}

    //! @name Accessors
    //@{
    MIL_Population&    GetPopulation     () const;
    MT_Float           GetDominationState() const;
    bool               HasStateChanged   () const; // Etat decisionnel
    virtual DEC_AutomateDecision* GetDecAutomate() const;
    virtual std::string GetName() const;
    //@}

    //! @name Notifications
    //@{
    void NotifyDominationStateChanged( MT_Float rValue );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgPopulationUpdate& msg ) const ;
    void SendFullState   ( NET_ASN_MsgPopulationUpdate& msg ) const ;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void      EndCleanStateAfterCrash      ();
    
    virtual void RegisterUserFunctions( directia::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void RegisterSelf( directia::Brain& brain );
    //@}

private:
    DIA_Parameters                       missionBehaviorParameters_;   

    MT_Float                             rDominationState_;
    MT_Float                             rLastDominationState_;
    bool                                 bStateHasChanged_;
    std::string                          name_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Population
    static int nDIANameIdx_;
};

#endif // __DEC_PopulationDecision_h_
