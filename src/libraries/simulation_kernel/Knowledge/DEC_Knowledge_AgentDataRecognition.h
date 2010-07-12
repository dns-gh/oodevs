// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataRecognition.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:01 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentDataRecognition.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentDataRecognition_h_
#define __DEC_Knowledge_AgentDataRecognition_h_

#include "DEC_Knowledge_Def.h"

namespace MsgsSimToClient
{
    class MsgUnitKnowledgeUpdate;
}

class DEC_Knowledge_AgentPerceptionDataRecognition;
class MIL_AgentType_ABC;
class MIL_Army_ABC;
class MIL_Agent_ABC;
class PHY_NatureAtlas;

// =============================================================================
// Created: NLD 2004-03-11
// Modified: JVT 2004-12-09
// =============================================================================
class DEC_Knowledge_AgentDataRecognition : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_AgentDataRecognition();
    virtual ~DEC_Knowledge_AgentDataRecognition();
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
    void Update( const DEC_Knowledge_AgentPerceptionDataRecognition& data );
    void Update( const DEC_Knowledge_AgentDataRecognition& data );
    void Extrapolate( const MIL_Agent_ABC& agentKnown );
    //@}

    //! @name Network
    //@{
    bool HasChanged() const;
    void SendChangedState( MsgsSimToClient::MsgUnitKnowledgeUpdate& asnMsg ) const;
    void SendFullState( MsgsSimToClient::MsgUnitKnowledgeUpdate& asnMsg ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetTimeLastUpdate() const;
    MT_Float GetOperationalState() const;
    MT_Float GetMajorOperationalState() const;
    const T_KnowledgeComposanteVector& GetComposantes() const;
    const MIL_Army_ABC* GetArmy() const;
    const PHY_NatureAtlas& GetNatureAtlas() const;
    const MIL_AgentType_ABC* GetAgentType() const;
    bool IsPC() const;
    bool IsHuman() const; // $$$$ NLD 2007-04-19: A CHIER
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
    MT_Float rOperationalState_;
    MT_Float rMajorOperationalState_;
    T_KnowledgeComposanteVector composantes_;
    const MIL_Army_ABC* pArmy_;
    bool bIsPC_;
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'
    // Network
    bool bOperationalStateChanged_;
    bool bAgentTypeUpdated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentDataRecognition )

#endif // __DEC_Knowledge_AgentDataRecognition_h_
