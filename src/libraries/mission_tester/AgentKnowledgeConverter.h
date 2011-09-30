// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentKnowledgeConverter_h_
#define __ActionParameterAgentKnowledgeConverter_h_

#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "AgentKnowledge.h"

namespace kernel
{
    class Agent_ABC;
}

namespace sword
{
    class UnitKnowledgeId;
}

namespace mission_tester
{
// =============================================================================
/** @class  AgentKnowledgeConverter
    @brief  AgentKnowledgeConverter
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class AgentKnowledgeConverter : public kernel::AgentKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    AgentKnowledgeConverter() {}
    virtual ~AgentKnowledgeConverter() {}
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC*   FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const;
    actions::parameters::AgentKnowledge*        FindAgent( const kernel::Agent_ABC* agent );
    void                                        AddAgent( const kernel::Agent_ABC* agent, actions::parameters::AgentKnowledge* knowledge, unsigned int id );
    //const kernel::PopulationKnowledge* FindPopulation( unsigned long id, const kernel::Entity_ABC& owner ) const = 0;
    //virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const;
    //virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner ) const;
    //const kernel::PopulationKnowledge* Find( const kernel::PopulationKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const = 0;
    //const kernel::PopulationKnowledge* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner ) const = 0;
    //@}
private:
    //! @name Member data
    //@{
    typedef std::map< unsigned int, std::pair< kernel::Agent_ABC*, actions::parameters::AgentKnowledge* > > T_Knowledges;
    typedef T_Knowledges::iterator                                                                          IT_Knowledges;
    T_Knowledges knowledges_;
    //@}
};
}

#endif // __ActionParameterAgentKnowledgeConverter_h_
