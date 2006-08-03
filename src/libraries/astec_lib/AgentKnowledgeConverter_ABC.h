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

class KnowledgeGroup;
class Agent_ABC;
class AgentKnowledge;
class Population;
class PopulationKnowledge;

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
    virtual const AgentKnowledge*      Find( const AgentKnowledge& base, const KnowledgeGroup& owner ) = 0;
    virtual const AgentKnowledge*      Find( const Agent_ABC& base,          const KnowledgeGroup& owner ) = 0;
    virtual const PopulationKnowledge* Find( const PopulationKnowledge& base, const KnowledgeGroup& owner ) = 0;
    virtual const PopulationKnowledge* Find( const Population& base,          const KnowledgeGroup& owner ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgeConverter_ABC( const AgentKnowledgeConverter_ABC& );            //!< Copy constructor
    AgentKnowledgeConverter_ABC& operator=( const AgentKnowledgeConverter_ABC& ); //!< Assignement operator
    //@}
};

#endif // __AgentKnowledgeConverter_ABC_h_
