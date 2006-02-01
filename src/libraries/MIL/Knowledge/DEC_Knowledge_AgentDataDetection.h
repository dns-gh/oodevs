// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataDetection.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:02 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_AgentDataDetection.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentDataDetection_h_
#define __DEC_Knowledge_AgentDataDetection_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_AgentPerceptionDataDetection;
class MIL_KnowledgeGroupType;
class PHY_Posture;
class MIL_Agent_ABC;

struct ASN1T_MsgUnitKnowledgeUpdate;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentDataDetection
{
    MT_COPYNOTALLOWED( DEC_Knowledge_AgentDataDetection ) 

public:
     DEC_Knowledge_AgentDataDetection();
    ~DEC_Knowledge_AgentDataDetection();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare    ();
    void Update     ( const DEC_Knowledge_AgentPerceptionDataDetection& data );
    void Update     ( const DEC_Knowledge_AgentDataDetection&           data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D&           GetPosition                   () const;
    const MT_Vector2D&           GetDirection                  () const;
          MT_Float               GetSpeed                      () const;
          MT_Float               GetAltitude                   () const;
    const T_ComposanteVolumeSet& GetVisionVolumes              () const;
    const PHY_Posture&           GetLastPosture                () const;
    const PHY_Posture&           GetCurrentPosture             () const;
          MT_Float               GetPostureCompletionPercentage() const;
          uint                   GetTimeLastUpdate             () const;
          bool                   IsSurrendered                 () const;
          bool                   IsPrisoner                    () const;
          bool                   IsRefugeeManaged              () const;
          bool                   IsDead                        () const;
    //@}

    //! @name Network
    //@{
    bool HasChanged      () const;
    void SendChangedState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const;
    void SendFullState   ( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void DoUpdate( const T& data );
    //@}

private:
    uint nTimeLastUpdate_;

    MT_Vector2D vPosition_;
    MT_Vector2D vDirection_;
    MT_Float    rSpeed_;
    MT_Float    rAltitude_;
    bool        bSurrendered_;
    bool        bPrisoner_; 
    bool        bRefugeeManaged_;
    bool        bDead_;

    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumeSet visionVolumes_;
    const PHY_Posture*    pLastPosture_;
    const PHY_Posture*    pCurrentPosture_;
    MT_Float              rPostureCompletionPercentage_;

    // Network
    bool bDirectionUpdated_;
    bool bSpeedUpdated_;
    bool bPositionUpdated_;
    bool bPrisonerUpdated_;
    bool bSurrenderedUpdated_;
    bool bRefugeeManagedUpdated_;
};

#include "DEC_Knowledge_AgentDataDetection.inl"

#endif // __DEC_Knowledge_AgentDataDetection_h_
