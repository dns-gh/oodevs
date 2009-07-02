// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerception.h $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_AgentPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentPerception_h_
#define __DEC_Knowledge_AgentPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "DEC_Knowledge_AgentPerceptionDataDetection.h"
#include "DEC_Knowledge_AgentPerceptionDataRecognition.h"
#include "DEC_Knowledge_AgentPerceptionDataIdentification.h"

class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;

// =============================================================================
/** @class  DEC_Knowledge_AgentPerception
    @brief  This class represents the perception of an agent 
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerception : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentPerception( const MIL_AgentPion& agentPerceiving, MIL_Agent_ABC& agentPerceived );
             DEC_Knowledge_AgentPerception();
    virtual ~DEC_Knowledge_AgentPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update ( const PHY_PerceptionLevel& perceptionLevel, bool bRecordModeEnabled );
    bool Clean  (); 
    //@}

    //! @name Attacker
    //@{
    void NotifyAttacker();
    bool IsAttacker    () const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

    //! @name Accessors
    //@{
          uint                 GetCreationTimeStep       () const;
          MIL_Agent_ABC&       GetAgentPerceived         () const;
    const MIL_AgentPion&       GetAgentPerceiving        () const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel () const;
    const PHY_PerceptionLevel& GetPreviousPerceptionLevel() const;
    const PHY_PerceptionLevel& GetMaxPerceptionLevel     () const;
          bool                 IsPerceived               () const; // Perception au tick courant
          
          bool                 IsAvailable               () const;
          void                 MakeAvailable             ( uint nDelay );

    const DEC_Knowledge_AgentPerceptionDataDetection&      GetDetectionData     () const;
    const DEC_Knowledge_AgentPerceptionDataRecognition&    GetRecognitionData   () const;
    const DEC_Knowledge_AgentPerceptionDataIdentification& GetIdentificationData() const;
    //@}

private:
          uint                                            nCreationTimeStep_;

          DEC_Knowledge_AgentPerceptionDataDetection      dataDetection_;
          DEC_Knowledge_AgentPerceptionDataRecognition    dataRecognition_;
          DEC_Knowledge_AgentPerceptionDataIdentification dataIdentification_;

    const MIL_AgentPion*                                  pAgentPerceiving_;
          MIL_Agent_ABC*                                  pAgentPerceived_;
    const PHY_PerceptionLevel*                            pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*                            pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel*                            pMaxPerceptionLevel_;

          bool                                            bRecordModeEnabled_;
          bool                                            bPreviousRecordModeEnabled_;
          uint                                            nRecordModeDisablingDelay_;

          bool                                            bAttacker_;
};

#endif // __DEC_Knowledge_AgentPerception_h_
