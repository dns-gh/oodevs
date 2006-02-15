//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationFlowKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationFlowKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationFlowKnowledge_h_
#define __PopulationFlowKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "OptionalValue.h"

class KnowledgeGroup;
class PopulationFlow;
class PopulationKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationFlowKnowledge
{
    MT_COPYNOTALLOWED( PopulationFlowKnowledge );
    friend class PopulationKnowledgePanel;

public:
    //! @name Types
    //@{
    struct FlowPart
    {
        FlowPart( ASN1T_PortionFlux& asn );
        T_PointVector flowPart_;
        MT_Float      rRelevance_;
    };

    typedef std::vector< FlowPart >       T_FlowParts;
    typedef T_FlowParts::const_iterator CIT_FlowParts;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     PopulationFlowKnowledge( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg );
    ~PopulationFlowKnowledge();
    //@}

    //! @name Accessors
    //@{
    uint                           GetID                 () const;
    const KnowledgeGroup&                GetKnowledgeGroup               () const;
    const PopulationKnowledge* GetPopulationKnowledge() const;
    const PopulationFlow*      GetFlow               () const;
    MT_Float                       GetDirection          () const;
    MT_Float                       GetSpeed              () const;
    uint                           GetNbrAliveHumans     () const;
    uint                           GetNbrDeadHumans      () const;
    E_PopulationAttitude           GetAttitude           () const;
    MT_Float                       GetRelevance          () const;
    bool                           IsPerceived           () const;
    const T_FlowParts&             GetFlowParts          () const;
    //@}


    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg );
    //@}

private:
    const uint                                   nID_;
          KnowledgeGroup*                              pKnowledgeGroup_;
    const PopulationKnowledge*               pPopulationKnowledge_;
    const PopulationFlow*                    pFlow_;
          OptionalValue< MT_Float >              rDirection_;
          OptionalValue< MT_Float >              rSpeed_;
          OptionalValue< uint     >              nNbrAliveHumans_;
          OptionalValue< uint     >              nNbrDeadHumans_;
          OptionalValue< E_PopulationAttitude >  eAttitude_;
          OptionalValue< bool     >              bIsPerceived_;
          OptionalValue< T_FlowParts >           flowParts_;
};


#include "PopulationFlowKnowledge.inl"

#endif // __PopulationFlowKnowledge_h_