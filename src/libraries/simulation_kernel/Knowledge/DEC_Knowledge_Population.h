// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Population.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_Population.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_Population_h_
#define __DEC_Knowledge_Population_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_KnowledgeGroup;
class MIL_Population;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class DEC_Knowledge_PopulationConcentration;
class DEC_Knowledge_PopulationFlow;
class DEC_Knowledge_PopulationPerception;
class DEC_Knowledge_PopulationConcentrationPerception;
class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_PopulationCollision;


// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_Population : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Population( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationKnown );
             DEC_Knowledge_Population();
    virtual ~DEC_Knowledge_Population();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive& , const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    
    void Update ( const DEC_Knowledge_PopulationPerception& perception );
    void Update ( const DEC_Knowledge_PopulationConcentrationPerception& perception ); // Called by DEC_Knowledge_PopulationPerception
    void Update ( const DEC_Knowledge_PopulationFlowPerception&          perception ); // Called by DEC_Knowledge_PopulationPerception

    void Update ( const DEC_Knowledge_PopulationCollision&  collision  );
    void Update ( const DEC_Knowledge_PopulationCollision&  collision, const MIL_PopulationFlow&          flow          ); // Called by DEC_Knowledge_PopulationCollision
    void Update ( const DEC_Knowledge_PopulationCollision&  collision, const MIL_PopulationConcentration& concentration ); // Called by DEC_Knowledge_PopulationCollision

    void UpdateRelevance();

    bool Clean  ();

    void Recon      ();
    void Secure     ( const MIL_AgentPion& pionSecuring );
    void Exterminate( const MIL_AgentPion& exterminator, MT_Float rSurface );
    //@}

    //! @name Accessors
    //@{
          uint                    GetID             () const;
    const MIL_Army_ABC&           GetArmy           () const;
    const MIL_KnowledgeGroup&     GetKnowledgeGroup () const;
          MIL_Population&         GetPopulationKnown() const;
          MT_Float                GetDangerosity    ( const MIL_AgentPion&    target        ) const;
          bool                    IsRecon           () const;
          bool                    IsInZone          ( const TER_Localisation& loc           ) const;
          MT_Float                GetDominationState() const;
    const MIL_PopulationAttitude& GetAttitude       () const;
          
          MT_Vector2D             GetClosestPoint   ( const MT_Vector2D&      refPos        ) const;
          MT_Vector2D             GetSecuringPoint  ( const MIL_Agent_ABC&    securingAgent ) const;
          MT_Vector2D             GetSafetyPosition ( const MIL_AgentPion&    agent        , MT_Float rMinDistance ) const;

    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    //! @name Network
    //@{
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

    //! @name Tools
    //@{
    DEC_Knowledge_PopulationConcentration& GetKnowledge         ( const MIL_PopulationConcentration& concentration );
    DEC_Knowledge_PopulationFlow&          GetKnowledge         ( const MIL_PopulationFlow&          flow          );
    void                                   UpdateReconAttributes();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint /*ID*/, DEC_Knowledge_PopulationConcentration* > T_ConcentrationMap;
    typedef T_ConcentrationMap::iterator                                    IT_ConcentrationMap;
    typedef T_ConcentrationMap::const_iterator                              CIT_ConcentrationMap;

    typedef std::map< uint /*ID*/, DEC_Knowledge_PopulationFlow* > T_FlowMap;
    typedef T_FlowMap::iterator                                    IT_FlowMap;
    typedef T_FlowMap::const_iterator                              CIT_FlowMap;
    //@}

private:
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
          MIL_Population*     pPopulationKnown_;
    const uint                nID_;
          T_ConcentrationMap  concentrations_;
          T_FlowMap           flows_;
          bool                bIsRecon_;

          bool                bReconAttributesValid_;
          MT_Float            rDominationState_;
          bool                bDecStateUpdated_;
};

#endif // __DEC_Knowledge_Population_h_
