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

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_AgentPerceptionDataIdentification;
class MIL_AgentType_ABC;
class MIL_Army;
class MIL_Agent_ABC;

struct ASN1T_MsgUnitKnowledgeUpdate;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentDataIdentification : private boost::noncopyable
{

public:
     DEC_Knowledge_AgentDataIdentification();
    ~DEC_Knowledge_AgentDataIdentification();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare    (); 
    void Update     ( const DEC_Knowledge_AgentPerceptionDataIdentification& data );
    void Update     ( const DEC_Knowledge_AgentDataIdentification&           data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Accessors
    //@{
          uint               GetTimeLastUpdate() const;
    const MIL_AgentType_ABC* GetAgentType     () const;
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
          uint               nTimeLastUpdate_;
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'

    // Network
    bool bAgentTypeUpdated_;
};

#include "DEC_Knowledge_AgentDataIdentification.inl"

#endif // __DEC_Knowledge_AgentDataIdentification_h_
