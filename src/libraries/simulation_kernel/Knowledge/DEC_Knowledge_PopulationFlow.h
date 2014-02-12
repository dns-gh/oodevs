// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlow.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlow.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationFlow_h_
#define __DEC_Knowledge_PopulationFlow_h_

#include "MIL.h"
#include "Tools/MIL_IDManager.h"
#include "DEC_Knowledge_PopulationFlowPart.h"

class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_PopulationCollision;
class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class PHY_PerceptionLevel;
class MIL_Agent_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlow : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationFlow& flowKnown );
             DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const DEC_Knowledge_PopulationFlow& knowledge );
             DEC_Knowledge_PopulationFlow();
    virtual ~DEC_Knowledge_PopulationFlow();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const DEC_Knowledge_PopulationFlowPerception& perception );
    void Update( const DEC_Knowledge_PopulationCollision& collision  );
    void UpdateFromCrowdPerception( int currentTimeStep );
    void UpdateRelevance();
    bool Clean();
    void HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel );
    double GetRelevance() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork() const;
    void SendStateToNewClient() const;
    //@}

private:
    //! @name Network
    //@{
    void SendFullState() const;
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Member data
    //@{
    const DEC_Knowledge_Population* pPopulationKnowledge_;
    const MIL_PopulationFlow* pFlowKnown_;
    const MIL_Population* pPopulation_;
    const unsigned int nID_;
    MT_Vector2D direction_;
    double rSpeed_;
    DEC_Knowledge_PopulationFlowPart flowParts_;
    unsigned int nNbrAliveHumans_;
    unsigned int nNbrDeadHumans_;
    const MIL_PopulationAttitude* pAttitude_;
    bool bReconAttributesValid_;
    // Network update
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;
    bool bRealFlowUpdated_;
    bool bFlowPartsUpdated_;
    bool bSpeedUpdated_;
    bool bDirectionUpdated_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pHackedPerceptionLevel_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationFlow )

#endif // __DEC_Knowledge_PopulationFlow_h_
