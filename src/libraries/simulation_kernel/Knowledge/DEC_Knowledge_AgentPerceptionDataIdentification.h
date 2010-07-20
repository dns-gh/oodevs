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
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_AgentType_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataIdentification : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentPerceptionDataIdentification();
    virtual ~DEC_Knowledge_AgentPerceptionDataIdentification();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetTimeLastUpdate() const;
    const MIL_AgentType_ABC* GetAgentType() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nTimeLastUpdate_;
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentPerceptionDataIdentification )

#endif // __DEC_Knowledge_AgentPerceptionDataIdentification_h_
