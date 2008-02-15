 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "game_asn/Asn.h"
#include "Position.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"
#include "Entity_ABC.h"
#include "DecisionalState.h"

namespace kernel
{
    class AgentType;
}

namespace dispatcher
{
    class Automat;
    class Model;
    class Equipment;
    class Dotation;
    class Humans;
    class Loan;
    class AgentLogMedical;
    class AgentLogMaintenance;
    class AgentLogSupply;
    class Side;
    class AgentOrder;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: NLD 2006-09-19
// =============================================================================
class Agent : public Entity_ABC
            , public Extension_ABC
            , public Updatable_ABC< ASN1T_MsgUnitCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( Model& model, const ASN1T_MsgUnitCreation& msg );
    virtual ~Agent();
    //@}

    //! @name Main
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitCreation& asnMsg );
    void Update( const ASN1T_MsgUnitAttributes&      asnMsg );
    void Update( const ASN1T_MsgDecisionalState&     asnMsg );
    void Update( const ASN1T_MsgLogMedicalState&     asnMsg );
    void Update( const ASN1T_MsgLogMaintenanceState& asnMsg );
    void Update( const ASN1T_MsgLogSupplyState&      asnMsg );
    void Update( const ASN1T_MsgUnitChangeSuperior&  asnMsg );
    void Update( const ASN1T_MsgUnitOrder&           asnMsg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    std::string GetName() const;
    const Automat& GetAutomat() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned int > T_OIDVector;
    //@}

public:
          Model&         model_;
    const unsigned long  nID_;
    const unsigned long  nType_; // XML reference - no resolved by dispatcher
    const kernel::AgentType& type_;
    const std::string    strName_;
          Automat*       pAutomat_;
    const bool           bPC_;

    Position                      position_;
    unsigned int                  nDirection_;
    unsigned int                  nHeight_;
    unsigned int                  nSpeed_;
    unsigned int                  nOperationalStateValue_;
    ModelRefsContainer< Agent >   reinforcements_;
    Agent*                        pReinforced_;
    bool                          bDead_;
    bool                          bNeutralized_;
    bool                          bStealthModeEnabled_;
    bool                          bLoaded_;
    bool                          bHumanTransportersAvailable_;
    ASN1T_EnumUnitPosture         nLastPosture_;
    ASN1T_EnumUnitPosture         nCurrentPosture_;
    unsigned int                  nPostureCompletion_;
    unsigned int                  nInstallationState_;
    bool                          bNbcProtectionSuitEnabled_;
    T_OIDVector                   nbcAgentTypesContaminating_;
    unsigned int                  nContaminationState_;
    bool                          bCommunicationJammed_;
    bool                          bBlackoutEnabled_;
    bool                          bRadarEnabled_;
    ModelRefsContainer< Agent >   transportedAgents_;
    Agent*                        pTransporter_;
    ASN1T_EnumForceRatioStatus          nForceRatioState_;
    ASN1T_EnumMeetingEngagementStatus nCloseCombatState_;
    ASN1T_EnumOperationalStatus    nOperationalState_;
    ASN1T_EnumFireAvailability    nIndirectFireAvailability_;
    ASN1T_EnumRoe                 nRoe_;
    ASN1T_EnumPopulationRoe       nPopulationRoe_;
    ASN1T_EnumUnitTiredness       nTiredness_;
    ASN1T_EnumUnitMorale          nMorale_;
    ASN1T_EnumUnitExperience      nExperience_;
    const Side*                   pSideSurrenderedTo_;
    bool                          bPrisonner_;
    bool                          bRefugeeManaged_;
    DecisionalState               decisionalInfos_;

    ModelsContainer< Equipment >  equipments_;
    ModelsContainer< Humans    >  troops_;
    ModelsContainer< Dotation  >  dotations_;
    ModelsContainer< Loan      >  borrowings_;
    ModelsContainer< Loan      >  lendings_;

    AgentLogMedical*              pLogMedical_;
    AgentLogMaintenance*          pLogMaintenance_;
    AgentLogSupply*               pLogSupply_;

    AgentOrder*                   pOrder_;
};

}

#include "Agent.inl"

#endif // __Agent_h_
