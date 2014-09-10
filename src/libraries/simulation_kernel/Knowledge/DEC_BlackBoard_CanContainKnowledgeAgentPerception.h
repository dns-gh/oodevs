// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h $
// $Author: Jvt $
// $Modtime: 6/04/05 19:10 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgentPerception.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_
#define __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_Def.h"
#include <tools/Map.h>

class DEC_Knowledge_AgentPerception;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeAgentPerception
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeAgentPerception : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgeAgentPerception();
    virtual ~DEC_BlackBoard_CanContainKnowledgeAgentPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_AgentPerception& CreateKnowledgeAgentPerception ( const MIL_Agent_ABC& agentPerceiving, const MIL_Agent_ABC& agentPerceived );
    void DestroyKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge );
    void CleanDeletedAgentKnowledges();
    void Clear();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_AgentPerception* GetKnowledgeAgentPerception( const MIL_Agent_ABC& associatedAgent ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgentPerception( UnaryFunction& fct ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); )
        {
            DEC_Knowledge_AgentPerception& knowledge = *it->second;
            ++it;
            fct( knowledge );
        }
    }

    template < class BinaryFunction, class Parameter >
    void ApplyOnKnowledgesAgentPerception( BinaryFunction& fct, Parameter param ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); )
        {
            DEC_Knowledge_AgentPerception& knowledge = *it->second;
            ++it;
            fct( knowledge, param );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    tools::Map< const MIL_Agent_ABC*, DEC_Knowledge_AgentPerception* > perceptions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeAgentPerception )

#endif // __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_
