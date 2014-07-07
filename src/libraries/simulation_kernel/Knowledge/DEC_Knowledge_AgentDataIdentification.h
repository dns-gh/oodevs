// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataIdentification.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:01 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_AgentDataIdentification.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentDataIdentification_h_
#define __DEC_Knowledge_AgentDataIdentification_h_

#include "MIL.h"

namespace sword
{
    class UnitKnowledgeUpdate;
}

class DEC_Knowledge_AgentPerceptionDataIdentification;
class MIL_AgentType_ABC;
class MIL_Army;
class MIL_Agent_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentDataIdentification : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentDataIdentification();
    virtual ~DEC_Knowledge_AgentDataIdentification();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const DEC_Knowledge_AgentPerceptionDataIdentification& data );
    void Update( const DEC_Knowledge_AgentDataIdentification& data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetTimeLastUpdate() const;
    const MIL_AgentType_ABC* GetAgentType() const;
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
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentDataIdentification )

#endif // __DEC_Knowledge_AgentDataIdentification_h_
