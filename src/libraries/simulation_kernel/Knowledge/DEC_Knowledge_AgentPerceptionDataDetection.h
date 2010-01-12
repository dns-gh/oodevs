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
class MIL_Army_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataDetection : private boost::noncopyable
{

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
    const MIL_Army_ABC*          GetArmySurrenderedTo          () const;
          bool                   IsPrisoner                    () const;
          bool                   IsRefugeeManaged              () const;
          bool                   IsDead                        () const;
    //@}

private:
    uint nTimeLastUpdate_;

    MT_Vector2D vPosition_;
    MT_Vector2D vDirection_;
    MT_Float    rAltitude_;
    MT_Float    rSpeed_;
    bool        bDead_;

    const MIL_Army_ABC* pArmySurrenderedTo_;
          bool      bPrisoner_; 
          bool      bRefugeeManaged_;
    
    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumeSet visionVolumes_;
    const PHY_Posture*    pLastPosture_;
    const PHY_Posture*    pCurrentPosture_;
    MT_Float              rPostureCompletionPercentage_;
};

#endif // __DEC_Knowledge_AgentPerceptionDataDetection_h_
