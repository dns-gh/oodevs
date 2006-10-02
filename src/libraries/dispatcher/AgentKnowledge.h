 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "AsnTypes.h"
#include "Position.h"

namespace dispatcher
{
    class Model;
    class Publisher_ABC;
    class KnowledgeGroup;
    class Agent;
    class Side;

// =============================================================================
/** @class  AgentKnowledge
    @brief  AgentKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
     AgentKnowledge( Model& model, const ASN1T_MsgUnitKnowledgeCreation& asnMsg );
    ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg );
    void SendCreation  ( Publisher_ABC& publisher );
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_AutomatePerception > T_AutomatePerceptionVector;

    //$$$ bullshit
    struct T_Optionals 
    {
      unsigned pertinencePresent : 1;
      unsigned identification_levelPresent : 1;
      unsigned max_identification_levelPresent : 1;
      unsigned etat_opPresent : 1;
      unsigned mortPresent : 1;
      unsigned positionPresent : 1;
      unsigned directionPresent : 1;
      unsigned speedPresent : 1;
      unsigned campPresent : 1;
      unsigned nature_armePresent : 1;
      unsigned nature_niveauPresent : 1;
      unsigned nature_pcPresent : 1;
      unsigned nature_specialisationPresent : 1;
      unsigned nature_qualificationPresent : 1;
      unsigned nature_categoriePresent : 1;
      unsigned nature_mobilitePresent : 1;
      unsigned capacite_missionPresent : 1;
      unsigned perception_par_compagniePresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonnierPresent : 1;
      unsigned refugie_pris_en_comptePresent : 1;
    };
    //@}

private:
          Model&          model_;
    const unsigned int    nID_; 
    const KnowledgeGroup& knowledgeGroup_;
    const Agent&          agent_;        

    unsigned int                        nRelevance_;
    ASN1T_EnumUnitIdentificationLevel   nPerceptionLevel_;
    ASN1T_EnumUnitIdentificationLevel   nMaxPerceptionLevel_;
    unsigned int                        nOperationalState_;
    bool                                bDead_;
    Position                            position_;
    unsigned int                        nDirection_;
    unsigned int                        nSpeed_;
    Side*                               pSide_;
    ASN1T_EnumUnitNatureWeapon          nNatureWeapon_;
    ASN1T_EnumNatureLevel               nNatureLevel_;
    ASN1T_EnumUnitNatureSpecialization  nNatureSpecialization_;
    ASN1T_EnumUnitNatureQualifier       nNatureQualifier_;
    ASN1T_EnumUnitNatureCategory        nNatureCategory_;
    ASN1T_EnumUnitNatureMobility        nNatureMobility_;
    ASN1T_EnumUnitCapaciteMission       nMissionCapability_;
    bool                                bPC_; //$$$ A VIRER
    T_AutomatePerceptionVector          automatePerceptions_;
    bool                                bSurrendered_;
    bool                                bPrisoner_;
    bool                                bRefugeeManaged_;

    T_Optionals                         optionals_;
};

}

#endif // __AgentKnowledge_h_
