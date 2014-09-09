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

#include "DEC_KnowledgeSource_ABC.h"
#include "MT_Tools/MT_Vector2DTypes.h"
#include <tools/Map.h>

class DEC_KnowledgeBlackBoard_AgentPion;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_PopulationPerception;
class MIL_Agent_ABC;
class MIL_Object_ABC;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class PHY_PerceptionLevel;
class MIL_UrbanObject_ABC;

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
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_KS_Perception( DEC_KnowledgeBlackBoard_AgentPion& blackBoard );
             DEC_KS_Perception();
    virtual ~DEC_KS_Perception();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
    bool HasDelayedPerceptions() const;
    //@}

    //! @name Events
    //@{
    // Called by ::Talk()
    // Return true if a new perception is created
    bool NotifyPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled );
    void NotifyPerception( MIL_Object_ABC& objectPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled );
    bool NotifyPerception( MIL_PopulationConcentration& concentrationPerceived, const PHY_PerceptionLevel& level, bool bRecordModeEnabled );
    bool NotifyPerception( MIL_PopulationFlow& flowPerceived, const PHY_PerceptionLevel& level, const T_PointVector& shape, bool bRecordModeEnabled );
    void NotifyPerception( const MIL_UrbanObject_ABC& object, const PHY_PerceptionLevel& level );
    // Not called by ::Talk()
    void NotifyExternalPerception( MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& level );
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk( int currentTimeStep );
    virtual void Clean();
    virtual void CleanDeletedAgentKnowledges();
    void Clear();
    void MakePerceptionsAvailable();
    void MakePerceptionsAvailableTimed();
    //@}

private:
    //! @name Operations
    //@{
    void CleanKnowledgeAgentPerception     ( DEC_Knowledge_AgentPerception&      knowledge );
    void CleanKnowledgeObjectPerception    ( DEC_Knowledge_ObjectPerception&     knowledge );
    void CleanKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge );
    //@}

private:
    //! @name
    //@{
    typedef tools::Map< MIL_Agent_ABC*, const PHY_PerceptionLevel* > T_AgentPerceptionMap;
    //@}

private:
    DEC_KnowledgeBlackBoard_AgentPion* pBlackBoard_;
    T_AgentPerceptionMap externalPerceptions_;
    bool bMakePerceptionsAvailable_;
    bool bMakePerceptionsAvailableTimed_;
};

BOOST_CLASS_EXPORT_KEY( DEC_KS_Perception )

#endif // __DEC_KS_Perception_h_
