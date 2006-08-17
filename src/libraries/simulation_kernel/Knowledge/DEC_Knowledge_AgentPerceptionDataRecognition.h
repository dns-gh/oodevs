// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataRecognition.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:56 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataRecognition.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_AgentPerceptionDataRecognition_h_
#define __DEC_Knowledge_AgentPerceptionDataRecognition_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_Army;
class MIL_AgentType_ABC;


// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataRecognition
{
    MT_COPYNOTALLOWED( DEC_Knowledge_AgentPerceptionDataRecognition ) 

public:
     DEC_Knowledge_AgentPerceptionDataRecognition();
    ~DEC_Knowledge_AgentPerceptionDataRecognition();

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
          uint                         GetTimeLastUpdate       () const;
          MT_Float                     GetOperationalState     () const;
          MT_Float                     GetMajorOperationalState() const;
    const T_KnowledgeComposanteVector& GetComposantes          () const;
    const MIL_Army*                    GetArmy                 () const;
          bool                         IsPC                    () const;
    const MIL_AgentType_ABC*           GetAgentType            () const;
    //@}

private:
          uint                         nTimeLastUpdate_;
          MT_Float                     rOperationalState_;
          MT_Float                     rMajorOperationalState_;
          T_KnowledgeComposanteVector  composantes_;
    const MIL_Army*                    pArmy_ ;
          bool                         bIsPC_;
    const MIL_AgentType_ABC*           pAgentType_; // For 'natures'
};

#include "DEC_Knowledge_AgentPerceptionDataRecognition.inl"

#endif // __DEC_Knowledge_AgentPerceptionDataRecognition_h_
