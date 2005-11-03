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

#include "Tools/MIL_MOSIDManager.h"

class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_PopulationFlowPart;
class DEC_Knowledge_PopulationCollision;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class PHY_PerceptionLevel;
class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlow
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationFlow ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationFlow& flowKnown );
             DEC_Knowledge_PopulationFlow();
    virtual ~DEC_Knowledge_PopulationFlow();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare        ();
    void Update         ( const DEC_Knowledge_PopulationFlowPerception& perception );
    void Update         ( const DEC_Knowledge_PopulationCollision&      collision  );
    void UpdateRelevance();
    bool Clean          ();
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    //! @name Network
    //@{
    void SendFullState     () const;
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_AgentPion*, DEC_Knowledge_PopulationFlowPart* > T_FlowPartMap;
    typedef T_FlowPartMap::iterator                                             IT_FlowPartMap;
    typedef T_FlowPartMap::const_iterator                                       CIT_FlowPartMap;
    //@}

private:
    const DEC_Knowledge_Population*  pPopulationKnowledge_;
    const MIL_PopulationFlow*        pFlowKnown_;
    const uint                       nID_;

          MT_Vector2D                direction_;
          MT_Float                   rSpeed_;
          T_FlowPartMap              flowParts_;

          uint                       nNbrAliveHumans_;
          uint                       nNbrDeadHumans_;
    const MIL_PopulationAttitude*    pAttitude_;
          bool                       bReconAttributesValid_;

          // Network update
          bool                       bHumansUpdated_;
          bool                       bAttitudeUpdated_;
          bool                       bRealFlowUpdated_;
          bool                       bFlowPartsUpdated_;
          bool                       bSpeedUpdated_;
          bool                       bDirectionUpdated_;
        
    const PHY_PerceptionLevel*       pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel*       pCurrentPerceptionLevel_;

private:
    static MIL_MOSIDManager          idManager_;

};

#include "DEC_Knowledge_PopulationFlow.inl"

#endif // __DEC_Knowledge_PopulationFlow_h_
