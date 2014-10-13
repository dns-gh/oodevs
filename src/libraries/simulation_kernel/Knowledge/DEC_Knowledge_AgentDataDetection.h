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
class PHY_PerceptionLevel;
class PHY_Posture;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class PHY_Volume;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentDataDetection : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< const PHY_Volume* > T_ComposanteVolumes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentDataDetection();
    virtual ~DEC_Knowledge_AgentDataDetection();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& a, unsigned int );

    void WriteKnowledges( xml::xostream& xos ) const;
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
    boost::shared_ptr< MT_Vector2D > GetPositionPtr() const;
    const MT_Vector2D& GetDirection() const;
    double GetSpeed() const;
    double GetAltitude() const;
    const T_ComposanteVolumes& GetVisibleVolumes() const;
    const PHY_Posture& GetLastPosture() const;
    const PHY_Posture& GetCurrentPosture() const;
    double GetPostureCompletionPercentage() const;
    unsigned int GetTimeLastUpdate() const;
    const MIL_Army_ABC* GetArmySurrenderedTo() const;
    bool IsSurrendered() const;
    bool IsPrisoner() const;
    bool IsRefugeeManaged() const;
    bool IsDead() const;
    bool IsWounded() const;
    double GetOperationalState() const;
    const MIL_Army_ABC* GetArmy() const;
    bool IsPC() const;
    double GetPopulationDensity() const;
    //@}

    //! @name Network
    //@{
    bool HasChanged() const;
    void SendChangedState( sword::UnitKnowledgeUpdate& msg, const PHY_PerceptionLevel& level, bool levelChanged ) const;
    void SendFullState( sword::UnitKnowledgeUpdate& msg, const PHY_PerceptionLevel& level ) const;
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
    boost::shared_ptr< MT_Vector2D > vPosition_;
    MT_Vector2D vDirection_;
    double rSpeed_;
    double rAltitude_;
    double rPopulationDensity_;
    const MIL_Army_ABC* pArmySurrenderedTo_;
    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumes visionVolumes_;
    const PHY_Posture* pLastPosture_;
    const PHY_Posture* pCurrentPosture_;
    double rPostureCompletionPercentage_;
    const MIL_Army_ABC* pArmy_;
    double rOperationalState_;
    bool bIsPC_;
    bool bDead_;
    bool bWounded_;
    bool bPrisoner_;
    bool bRefugeeManaged_;
    bool bDirectionUpdated_;
    bool bSpeedUpdated_;
    bool bPositionUpdated_;
    bool bPrisonerUpdated_;
    bool bSurrenderedUpdated_;
    bool bRefugeeManagedUpdated_;
    bool bDeadUpdated_;
    bool bOperationalStateUpdated_;
    bool bArmyAndPcUpdated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentDataDetection )

#endif // __DEC_Knowledge_AgentDataDetection_h_
