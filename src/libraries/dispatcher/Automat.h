 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "game_asn/Asn.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"

namespace dispatcher
{
class Dispatcher;
class Side;
class KnowledgeGroup;
class Model;
class Formation;
class Agent;
class DotationQuota;
class Publisher_ABC;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( Model& model, const ASN1T_MsgAutomateCreation& msg );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    unsigned long                GetID    () const;
    ModelRefsContainer< Agent >& GetAgents();
    //@}

    //! @name Main
    //@{
    void Update        ( const ASN1T_MsgAutomateAttributes&                  msg );
    void Update        ( const ASN1T_MsgLogRavitaillementQuotas&             msg );
    void Update        ( const ASN1T_MsgAutomateChangeLiensLogistiquesAck&   msg );
    void Update        ( const ASN1T_MsgAutomateChangeLiensLogistiques&      msg );
    void Update        ( const ASN1T_MsgAutomateChangeGroupeConnaissanceAck& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

private:
          Model&                              model_;
    const unsigned long                       nID_;
    const unsigned long                       nType_; // XML reference - no resolved by dispatcher
    const std::string                         strName_;
          Side&                               side_;
          Formation&                          formation_;
          KnowledgeGroup*                     pKnowledgeGroup_;
          ModelRefsContainer< Agent >         agents_;
          ModelsContainer   < DotationQuota > quotas_;

    ASN1T_EnumAutomateState       nAutomatState_;
    ASN1T_EnumEtatRapFor          nForceRatioState_;
    ASN1T_EnumEtatCombatRencontre nCloseCombatState_;
    ASN1T_EnumEtatOperationnel    nOperationalState_;
    ASN1T_EnumRoe                 nRoe_;

    Automat* pTC2_;
    Automat* pLogMaintenance_;
    Automat* pLogMedical_;
    Automat* pLogSupply_;
};

}

#include "Automat.inl"

#endif // __Automat_h_
