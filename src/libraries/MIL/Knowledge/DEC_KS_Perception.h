// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Perception.h $
// $Author: Jvt $
// $Modtime: 5/04/05 18:36 $
// $Revision: 7 $
// $Workfile: DEC_KS_Perception.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Perception_h_
#define __DEC_KS_Perception_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_ObjectPerception;
class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_RealObject_ABC;
class PHY_PerceptionLevel;

// =============================================================================
/** @class  DEC_KS_Perception
    @brief  This knowledge source (KS) is an 'IN', which launches the perception
            of the associated agent to publish new knowledges (Knowledge_PerceptionXXX)
            into the associated black board
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Perception : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_Perception ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_Perception( DEC_KnowledgeBlackBoard& blackBoard, MIL_AgentPion& agentPerceiving );
             DEC_KS_Perception();
    virtual ~DEC_KS_Perception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( boost::archive::binary_iarchive&, const uint );
    void save( boost::archive::binary_oarchive&, const uint ) const;
    //@}


    //! @name Accessors
    //@{
    bool HasDelayedPerceptions() const;
    //@}
    
    //! @name Events
    //@{
    // Called by ::Talk()
    void NotifyAgentPerception ( MIL_Agent_ABC&      agentPerceived , const PHY_PerceptionLevel& level, bool bRecordModeEnabled );
    void NotifyObjectPerception( MIL_RealObject_ABC& objectPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled );

    // Not called by ::Talk()
    void NotifyAgentExternalPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level );
    //@}

    //! @name Operations
    //@{
    virtual void Prepare                      ();
    virtual void Talk                         ();
    virtual void Clean                        ();
            void MakePerceptionsAvailable     ();
            void MakePerceptionsAvailableTimed();
    //@}

private:
    //! @name Operations
    //@{
    void CleanKnowledgeAgentPerception ( DEC_Knowledge_AgentPerception& perception );
    void CleanKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& perception );
    //@}

public:
    //! @name 
    //@{
    typedef std::map< MIL_Agent_ABC*, const PHY_PerceptionLevel* > T_AgentPerceptionMap;
    typedef T_AgentPerceptionMap::const_iterator                   CIT_AgentPerceptionMap;
    //@}

private:
    T_AgentPerceptionMap    externalPerceptions_;
    MIL_AgentPion*          pAgentPerceiving_;
    bool                    bMakePerceptionsAvailable_;
    bool                    bMakePerceptionsAvailableTimed_;
};

#include "DEC_KS_Perception.inl"

#endif // __DEC_KS_Perception_h_
