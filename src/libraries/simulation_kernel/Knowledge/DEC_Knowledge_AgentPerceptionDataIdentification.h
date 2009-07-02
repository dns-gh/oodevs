// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataIdentification.h $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataIdentification.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentPerceptionDataIdentification_h_
#define __DEC_Knowledge_AgentPerceptionDataIdentification_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_AgentType_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataIdentification : private boost::noncopyable
{

public:
     DEC_Knowledge_AgentPerceptionDataIdentification();
    ~DEC_Knowledge_AgentPerceptionDataIdentification();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Accessors
    //@{
          uint               GetTimeLastUpdate() const;
    const MIL_AgentType_ABC* GetAgentType     () const;
    //@}

private:
          uint               nTimeLastUpdate_;
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'
};

#include "DEC_Knowledge_AgentPerceptionDataIdentification.inl"

#endif // __DEC_Knowledge_AgentPerceptionDataIdentification_h_
