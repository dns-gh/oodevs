//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: MOS_PopulationKnowledge.h $
//
//*****************************************************************************

#ifndef __MOS_PopulationKnowledge_h_
#define __MOS_PopulationKnowledge_h_

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_Gtia;
class MOS_Team;
class MOS_Population;
class MOS_PopulationConcentrationKnowledge;
class MOS_PopulationFlowKnowledge;
class MOS_TypePopulation;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class MOS_PopulationKnowledge
{
    MT_COPYNOTALLOWED( MOS_PopulationKnowledge );

public:
    //! @name Types
    //@{
    typedef std::map< uint, MOS_PopulationConcentrationKnowledge* > T_ConcentrationKnowledgeMap;
    typedef T_ConcentrationKnowledgeMap::iterator                   IT_ConcentrationKnowledgeMap;
    typedef T_ConcentrationKnowledgeMap::const_iterator             CIT_ConcentrationKnowledgeMap;

    typedef std::map< uint, MOS_PopulationFlowKnowledge* >          T_FlowKnowledgeMap;
    typedef T_FlowKnowledgeMap::iterator                            IT_FlowKnowledgeMap;
    typedef T_FlowKnowledgeMap::const_iterator                      CIT_FlowKnowledgeMap;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     MOS_PopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg );
    ~MOS_PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    const uint                         GetID            () const;
    const MOS_Team*                    GetTeam          () const;
    const MOS_Population&              GetPopulation    () const;
          MOS_Gtia*                    GetGtia          () const;
    const MOS_TypePopulation&          GetType          () const;
    const T_ConcentrationKnowledgeMap& GetConcentrations() const;
    const T_FlowKnowledgeMap&          GetFlows         () const;
    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg );

    void OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeCreation            ( const ASN1T_MsgPopulationFluxKnowledgeCreation&             asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeUpdate              ( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               asnMsg );
    void OnReceiveMsgPopulationFlowKnowledgeDestruction         ( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          asnMsg );
    //@}

private:
    const uint                          nID_;
    const MOS_Team*                     pTeam_;
    const MOS_Population*               pPopulation_;
    const MOS_TypePopulation*           pType_;
          MOS_Gtia*                     pGtia_;

          T_ConcentrationKnowledgeMap   concentrations_;
          T_FlowKnowledgeMap            flows_;
};


#	include "MOS_PopulationKnowledge.inl"

#endif // __MOS_PopulationKnowledge_h_