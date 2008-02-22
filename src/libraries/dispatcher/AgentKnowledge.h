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

#include "game_asn/Asn.h"
#include "Position.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Model;
    class KnowledgeGroup;
    class Agent;
    class Side;

// =============================================================================
/** @class  AgentKnowledge
    @brief  AgentKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentKnowledge : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledge( Model& model, const ASN1T_MsgUnitKnowledgeCreation& asnMsg );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    using Entity_ABC::Update;
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    std::string GetName() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_AutomatPerception > T_AutomatePerceptionVector;

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
      unsigned nature_pcPresent : 1;
      unsigned perception_par_compagniePresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonnierPresent : 1;
      unsigned refugie_pris_en_comptePresent : 1;
    };
    //@}

public:
          Model&          model_;
    const unsigned int    nID_;
    const KnowledgeGroup& knowledgeGroup_;
    const Agent&          agent_;
    const ASN1T_UnitType  nTypeAgent_;

    unsigned int                        nRelevance_;
    ASN1T_EnumUnitIdentificationLevel   nPerceptionLevel_;
    ASN1T_EnumUnitIdentificationLevel   nMaxPerceptionLevel_;
    unsigned int                        nOperationalState_;
    bool                                bDead_;
    Position                            position_;
    unsigned int                        nDirection_;
    unsigned int                        nSpeed_;
    Side*                               pSide_;
    bool                                bPC_; //$$$ A VIRER
    T_AutomatePerceptionVector          automatePerceptions_;
    bool                                bSurrendered_;
    bool                                bPrisoner_;
    bool                                bRefugeeManaged_;

    T_Optionals                         optionals_;
};

}

#endif // __AgentKnowledge_h_
