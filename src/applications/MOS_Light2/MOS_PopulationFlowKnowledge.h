//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationFlowKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: MOS_PopulationFlowKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_PopulationFlowKnowledge_h_
#define __MOS_PopulationFlowKnowledge_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"
#include "OptionalValue.h"

class MOS_Gtia;
class MOS_PopulationFlow;
class MOS_PopulationKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class MOS_PopulationFlowKnowledge
{
    MT_COPYNOTALLOWED( MOS_PopulationFlowKnowledge );
    friend class MOS_PopulationKnowledgePanel;

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
     MOS_PopulationFlowKnowledge( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg );
    ~MOS_PopulationFlowKnowledge();
    //@}

    //! @name Accessors
    //@{
    uint                           GetID                 () const;
    const MOS_Gtia&                GetGtia               () const;
    const MOS_PopulationKnowledge* GetPopulationKnowledge() const;
    const MOS_PopulationFlow*      GetFlow               () const;
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
          MOS_Gtia*                              pGtia_;
    const MOS_PopulationKnowledge*               pPopulationKnowledge_;
    const MOS_PopulationFlow*                    pFlow_;
          OptionalValue< MT_Float >              rDirection_;
          OptionalValue< MT_Float >              rSpeed_;
          OptionalValue< uint     >              nNbrAliveHumans_;
          OptionalValue< uint     >              nNbrDeadHumans_;
          OptionalValue< E_PopulationAttitude >  eAttitude_;
          OptionalValue< bool     >              bIsPerceived_;
          OptionalValue< T_FlowParts >           flowParts_;
};


#include "MOS_PopulationFlowKnowledge.inl"

#endif // __MOS_PopulationFlowKnowledge_h_