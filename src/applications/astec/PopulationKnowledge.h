//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "OptionalValue.h"

class Gtia;
class Team;
class Population;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class TypePopulation;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge
{
    MT_COPYNOTALLOWED( PopulationKnowledge );

    friend class PopulationKnowledgePanel;

public:
    //! @name Types
    //@{
    typedef std::map< uint, PopulationConcentrationKnowledge* > T_ConcentrationKnowledgeMap;
    typedef T_ConcentrationKnowledgeMap::iterator                   IT_ConcentrationKnowledgeMap;
    typedef T_ConcentrationKnowledgeMap::const_iterator             CIT_ConcentrationKnowledgeMap;

    typedef std::map< uint, PopulationFlowKnowledge* >          T_FlowKnowledgeMap;
    typedef T_FlowKnowledgeMap::iterator                            IT_FlowKnowledgeMap;
    typedef T_FlowKnowledgeMap::const_iterator                      CIT_FlowKnowledgeMap;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     PopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg );
    ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    const uint                         GetID            () const;
    const Team*                        GetTeam          () const;
    const Population&                  GetPopulation    () const;
          Gtia*                        GetGtia          () const;
    const TypePopulation&              GetType          () const;
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
    const uint                      nID_;
    const Team*                     pTeam_;
    const Population*               pPopulation_;
    const TypePopulation*           pType_;
          Gtia*                     pGtia_;

          T_ConcentrationKnowledgeMap   concentrations_;
          T_FlowKnowledgeMap            flows_;

          OptionalValue< int >  nDomination_;
};


#	include "PopulationKnowledge.inl"

#endif // __PopulationKnowledge_h_