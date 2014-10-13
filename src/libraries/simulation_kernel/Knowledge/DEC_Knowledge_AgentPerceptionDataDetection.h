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

#include "DEC_Knowledge_Def.h"
#include "MIL.h"
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class PHY_Posture;
class MIL_Army_ABC;
class PHY_Volume;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataDetection : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< const PHY_Volume* > T_ComposanteVolumes;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentPerceptionDataDetection();
    virtual ~DEC_Knowledge_AgentPerceptionDataDetection();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare( const MIL_Agent_ABC& agentPerceived );
    void Update ( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetTimeLastUpdate() const;
    const MT_Vector2D& GetPosition () const;
    const MT_Vector2D& GetDirection() const;
    double GetSpeed() const;
    double GetAltitude() const;
    const T_ComposanteVolumes& GetVisibleVolumes() const;
    const PHY_Posture& GetLastPosture() const;
    const PHY_Posture& GetCurrentPosture() const;
    double GetPostureCompletionPercentage() const;
    const MIL_Army_ABC* GetArmySurrenderedTo() const;
    const MIL_Army_ABC* GetArmy() const;
    bool IsPC() const;
    bool IsPrisoner() const;
    bool IsRefugeeManaged() const;
    bool IsDead() const;
    bool IsWounded() const;
    double GetPopulationDensity() const;
    double GetOperationalState() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nTimeLastUpdate_;
    MT_Vector2D vPosition_;
    MT_Vector2D vDirection_;
    double rAltitude_;
    double rSpeed_;
    double rPopulationDensity_;
    double rOperationalState_;
    const MIL_Army_ABC* pArmySurrenderedTo_;
    // Attributes used by the vision, to see if we see a knowledge which doesn't exist anymore
    T_ComposanteVolumes visionVolumes_;
    const PHY_Posture* pLastPosture_;
    const PHY_Posture* pCurrentPosture_;
    double rPostureCompletionPercentage_;
    const MIL_Army_ABC* pArmy_;
    bool bIsPC_;
    bool bDead_;
    bool bPrisoner_;
    bool bRefugeeManaged_;
    bool bWounded_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentPerceptionDataDetection )

#endif // __DEC_Knowledge_AgentPerceptionDataDetection_h_
