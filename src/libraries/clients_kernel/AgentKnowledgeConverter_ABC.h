// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeConverter_ABC_h_
#define __AgentKnowledgeConverter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Population_ABC;
    class AgentKnowledge_ABC;
    class PopulationKnowledge_ABC;

// =============================================================================
/** @class  AgentKnowledgeConverter_ABC
    @brief  AgentKnowledgeConverter_ABC
*/
// Created: AGE 2006-05-18
// =============================================================================
class AgentKnowledgeConverter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeConverter_ABC() {};
    virtual ~AgentKnowledgeConverter_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC*      FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) = 0;
    virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long id, const kernel::Entity_ABC& owner ) = 0;
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& base, const kernel::Entity_ABC& owner ) = 0;
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner ) = 0;
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& base, const kernel::Entity_ABC& owner ) = 0;
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner ) = 0;
    //@}
};

}

#endif // __AgentKnowledgeConverter_ABC_h_
