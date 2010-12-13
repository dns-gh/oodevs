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

namespace sword
{
    class UnitKnowledgeUpdate;
}

class DEC_Knowledge_AgentPerceptionDataDetection;
class MIL_KnowledgeGroupType;
class PHY_Posture;
class MIL_Agent_ABC;
class MIL_Army_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentDataDetection : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentDataDetection();
    virtual ~DEC_Knowledge_AgentDataDetection();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const DEC_Knowledge_AgentPerceptionDataDetection& data );
    void Update( const DEC_Knowledge_AgentDataDetection& data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Accessors
    //@{
    const MT_Vector2D& GetPosition() const;
    const MT_Vector2D& GetDirection() const;
    double GetSpeed() const;
    double GetAltitude() const;
    const T_ComposanteVolumeSet& GetVisionVolumes() const;
    const PHY_Posture& GetLastPosture() const;
    const PHY_Posture& GetCurrentPosture() const;
    double GetPostureCompletionPercentage() const;
    unsigned int GetTimeLastUpdate() const;
    const MIL_Army_ABC* GetArmySurrenderedTo() const;
    bool IsSurrendered() const;
    bool IsPrisoner() const;
    bool IsRefugeeManaged() const;
    bool IsDead() const;
    //@}

    //! @name Network
    //@{
    bool HasChanged() const;
    void SendChangedState( sword::UnitKnowledgeUpdate& asnMsg ) const;
    void SendFullState( sword::UnitKnowledgeUpdate& asnMsg ) const;
    //@}

private:
    //! @name Tools
    //@{
    template< typename T > void DoUpdate( const T& data );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nTimeLastUpdate_;
    MT_Vector2D vPosition_;
    MT_Vector2D vDirection_;
    double rSpeed_;
    double rAltitude_;
    bool bDead_;
    const MIL_Army_ABC* pArmySurrenderedTo_;
    bool bPrisoner_;
    bool bRefugeeManaged_;
    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumeSet visionVolumes_;
    const PHY_Posture* pLastPosture_;
    const PHY_Posture* pCurrentPosture_;
    double rPostureCompletionPercentage_;
    // Network
    bool bDirectionUpdated_;
    bool bSpeedUpdated_;
    bool bPositionUpdated_;
    bool bPrisonerUpdated_;
    bool bSurrenderedUpdated_;
    bool bRefugeeManagedUpdated_;
    bool bDeadUpdated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentDataDetection )

#endif // __DEC_Knowledge_AgentDataDetection_h_
