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

#include "AsnTypes.h"
#include "Position.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"

namespace dispatcher
{
    class Dispatcher;
    class Automat;
    class Model;
    class Equipment;
    class Dotation;
    class Humans;
    class Loan;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: NLD 2006-09-19
// =============================================================================
class Agent
{
public:
    //! @name Constructors/Destructor
    //@{
     Agent( Model& model, const ASN1T_MsgPionCreation& msg );
    ~Agent();
    //@}

    //! @name Main
    //@{
    void Update( const ASN1T_MsgUnitAttributes& asnMsg );
    void Update( const ASN1T_MsgUnitDotations&  asnMsg );
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
    typedef std::vector< Loan >         T_LoanVector;
    //@}

private:
          Model&         model_;
    const unsigned long  nID_;
    const unsigned long  nType_; // XML reference - no resolved by dispatcher
    const std::string    strName_;
          Automat&       automat_;

    Position                      position_;
    unsigned int                  nDirection_;
    unsigned int                  nHeight_;
    unsigned int                  nSpeed_;
    unsigned int                  nOperationalStateValue_;
    ModelRefsContainer< Agent >   reinforcements_;
    Agent*                        pReinforced_;
    ASN1T_EnumAutomateState       nAutomatState_; //$$$ Séparation pions/automates
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
    bool                          bSurrendered_;
    bool                          bPrisonner_;
    bool                          bRefugeeManaged_;

    ModelsContainer< Equipment >  equipments_;
    ModelsContainer< Humans    >  troops_;
    ModelsContainer< Dotation  >  dotations_;

    T_LoanVector                  borrowings_;
    T_LoanVector                  lendings_;
};

}

#include "Agent.inl"

#endif // __Agent_h_
