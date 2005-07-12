// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataDetection.h $
// $Author: Jvt $
// $Modtime: 30/03/05 12:00 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataDetection.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentPerceptionDataDetection_h_
#define __DEC_Knowledge_AgentPerceptionDataDetection_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class PHY_Posture;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataDetection
{
    MT_COPYNOTALLOWED( DEC_Knowledge_AgentPerceptionDataDetection ) 

public:
     DEC_Knowledge_AgentPerceptionDataDetection();
    ~DEC_Knowledge_AgentPerceptionDataDetection();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare( const MIL_Agent_ABC& agentPerceived ); // 
    void Update ( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Accessors
    //@{
          uint                   GetTimeLastUpdate             () const;
    const MT_Vector2D&           GetPosition                   () const;
    const MT_Vector2D&           GetDirection                  () const;
          MT_Float               GetSpeed                      () const;
          MT_Float               GetAltitude                   () const;
    const T_ComposanteVolumeSet& GetVisionVolumes              () const;
    const PHY_Posture&           GetLastPosture                () const;
    const PHY_Posture&           GetCurrentPosture             () const;
          MT_Float               GetPostureCompletionPercentage() const;
          bool                   IsSurrendered                 () const;
          bool                   IsPrisoner                    () const;
          bool                   IsRefugeeManaged              () const;
    //@}

private:
    uint nTimeLastUpdate_;

    MT_Vector2D vPosition_;
    MT_Vector2D vDirection_;
    MT_Float    rAltitude_;
    MT_Float    rSpeed_;
    bool        bSurrendered_;
    bool        bPrisoner_; 
    bool        bRefugeeManaged_;

    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumeSet visionVolumes_;
    const PHY_Posture*    pLastPosture_;
    const PHY_Posture*    pCurrentPosture_;
    MT_Float              rPostureCompletionPercentage_;
};

#include "DEC_Knowledge_AgentPerceptionDataDetection.inl"

#endif // __DEC_Knowledge_AgentPerceptionDataDetection_h_
