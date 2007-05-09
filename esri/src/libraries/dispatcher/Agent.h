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

namespace dispatcher
{
class Dispatcher;
class Automat;
class Model;
class Equipment;
class Dotation;
class Humans;
class Loan;
class Publisher_ABC;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( Model& model, const ASN1T_MsgPionCreation& msg );
    virtual ~Agent();
    //@}

    //! @name Main
    //@{
    void Update        ( const ASN1T_MsgPionCreation&          asnMsg );
    void Update        ( const ASN1T_MsgUnitAttributes&        asnMsg );
    void Update        ( const ASN1T_MsgLogSanteEtat&          asnMsg );
    void Update        ( const ASN1T_MsgLogMaintenanceEtat&    asnMsg );
    void Update        ( const ASN1T_MsgLogRavitaillementEtat& asnMsg );
    void Update        ( const ASN1T_MsgPionChangeSuperior&    asnMsg );
    void Update        ( const ASN1T_MsgPionChangeSuperiorAck& asnMsg );
    void Update        ( const ASN1T_MsgPionOrder&             asnMsg );
    virtual void SendCreation   ( Publisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( Publisher_ABC& publisher ) const;
    virtual void SendSpecialUpdate( Publisher_ABC& publisher ) const;
    virtual void SendDestruction( Publisher_ABC& publisher ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
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

private:
          Model&         model_;
    const unsigned long  nID_;
    const unsigned long  nType_; // XML reference - no resolved by dispatcher
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
    ASN1T_EnumEtatRapFor          nForceRatioState_;
    ASN1T_EnumEtatCombatRencontre nCloseCombatState_;
    ASN1T_EnumEtatOperationnel    nOperationalState_;
    ASN1T_EnumDisponibiliteAuTir  nIndirectFireAvailability_;
    ASN1T_EnumRoe                 nRoe_;
    ASN1T_EnumRoePopulation       nRoePopulation_;
    ASN1T_EnumUnitFatigue         nTiredness_;
    ASN1T_EnumUnitMoral           nMorale_;
    ASN1T_EnumUnitExperience      nExperience_;
    const Side*                   pSideSurrenderedTo_;
    bool                          bPrisonner_;
    bool                          bRefugeeManaged_;

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
