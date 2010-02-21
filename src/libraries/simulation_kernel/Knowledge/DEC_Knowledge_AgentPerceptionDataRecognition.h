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
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_Army;
class MIL_Army_ABC;
class MIL_AgentType_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_AgentPerceptionDataRecognition : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
     DEC_Knowledge_AgentPerceptionDataRecognition();
    virtual ~DEC_Knowledge_AgentPerceptionDataRecognition();
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
          unsigned int                         GetTimeLastUpdate       () const;
          MT_Float                     GetOperationalState     () const;
          MT_Float                     GetMajorOperationalState() const;
    const T_KnowledgeComposanteVector& GetComposantes          () const;
    const MIL_Army_ABC*                GetArmy                 () const;
          bool                         IsPC                    () const;
    const MIL_AgentType_ABC*           GetAgentType            () const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nTimeLastUpdate_;
    MT_Float rOperationalState_;
    MT_Float rMajorOperationalState_;
    T_KnowledgeComposanteVector composantes_;
    const MIL_Army* pArmy_ ;
    bool bIsPC_;
    const MIL_AgentType_ABC* pAgentType_; // For 'natures'
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_AgentPerceptionDataRecognition )

#endif // __DEC_Knowledge_AgentPerceptionDataRecognition_h_
