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
#include "Tools/MIL_IDManager.h"

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
class PHY_PerceptionLevel;
class TER_Localisation;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_Population : public DEC_Knowledge_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_Population( const MIL_KnowledgeGroup& group, MIL_Population& populationKnown );
             DEC_Knowledge_Population( const DEC_Knowledge_Population& knowledge, const MIL_KnowledgeGroup& group );
             DEC_Knowledge_Population();
    virtual ~DEC_Knowledge_Population();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive& , const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, DEC_Knowledge_PopulationConcentration* > T_ConcentrationMap;
    typedef std::map< unsigned int, DEC_Knowledge_PopulationFlow* > T_FlowMap;
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, DEC_Knowledge_PopulationConcentration* > T_ConcentrationMap;

    typedef std::map< unsigned int, DEC_Knowledge_PopulationFlow* > T_FlowMap;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const DEC_Knowledge_PopulationPerception& perception );
    void Update( const DEC_Knowledge_PopulationConcentrationPerception& perception ); // Called by DEC_Knowledge_PopulationPerception
    void Update( const DEC_Knowledge_PopulationFlowPerception& perception ); // Called by DEC_Knowledge_PopulationPerception
    void Update( const DEC_Knowledge_PopulationCollision& collision );
    void Update( const DEC_Knowledge_PopulationCollision& collision, const MIL_PopulationFlow& flow ); // Called by DEC_Knowledge_PopulationCollision
    void Update( const DEC_Knowledge_PopulationCollision& collision, const MIL_PopulationConcentration& concentration ); // Called by DEC_Knowledge_PopulationCollision
    void Update( const DEC_Knowledge_Population& pKnowledge, int currentTimeStep );
    void UpdateFromCrowdPerception( const MIL_PopulationConcentration& concentration, int currentTimeStep );
    void UpdateFromCrowdPerception( const MIL_PopulationFlow& flow, int currentTimeStep );
    void UpdateRelevance();
    bool Clean();
    void Recon();
    void Secure( const MIL_AgentPion& pionSecuring );
    void Exterminate( const MIL_AgentPion& exterminator, double rSurface );
    void CopyFrom( const DEC_Knowledge_Population& knowledge );
    void Merge( const DEC_Knowledge_Population& knowledge );
    void SetCriticalIntelligenceFromPopulationKnown();
    void HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel );
    const PHY_PerceptionLevel* GetHackedPerceptionLevel() const;
    bool IsPerceptionDistanceHacked() const;
    void Lock();
    void Unlock();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const MIL_Army_ABC& GetArmy() const;
    double GetMaxLifeTime() const;
    const unsigned int GetKnowledgeGroupId() const;
    MIL_Population& GetPopulationKnown() const;
    double GetDangerosity( const MIL_AgentPion& target ) const;
    bool IsRecon() const;
    bool IsInZone( const TER_Localisation& loc ) const;
    double GetDominationState() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    MT_Vector2D GetClosestPoint( const MT_Vector2D& refPos ) const;
    boost::shared_ptr< MT_Vector2D > GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const;
    boost::shared_ptr< MT_Vector2D > GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const;
    const std::string& GetCriticalIntelligence() const;
    const T_ConcentrationMap& GetConcentrationMap() const;
    const T_FlowMap& GetFlowMap() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork() const;
    void SendStateToNewClient() const;
    //@}

private:
    //! @name Network
    //@{
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    //@}

    //! @name Tools
    //@{
    DEC_Knowledge_PopulationConcentration& GetKnowledge( const MIL_PopulationConcentration& concentration );
    DEC_Knowledge_PopulationFlow& GetKnowledge( const MIL_PopulationFlow& flow );
    void UpdateReconAttributes();
    //@}

private:
    //! @name Data Members
    //@{
    static MIL_IDManager idManager_;
    MIL_Population* pPopulationKnown_;
    const PHY_PerceptionLevel* pHackedPerceptionLevel_;
    std::string criticalIntelligence_;
    unsigned int nID_;
    unsigned int groupId_;
    T_ConcentrationMap concentrations_;
    T_FlowMap flows_;
    double rDominationState_;
    double maxLifetime_;
    int locked_;
    bool bIsRecon_;
    bool bReconAttributesValid_;
    bool bDecStateUpdated_;
    bool bCriticalIntelligenceUpdated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_Population )

#endif // __DEC_Knowledge_Population_h_
