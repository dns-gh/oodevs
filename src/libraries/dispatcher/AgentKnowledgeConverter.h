// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AgentKnowledgeConverter_h_
#define __AgentKnowledgeConverter_h_

#include "clients_kernel/AgentKnowledgeConverter_ABC.h"

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  AgentKnowledgeConverter
    @brief  Agent knowledge converter
*/
// Created: SBO 2010-05-11
// =============================================================================
class AgentKnowledgeConverter : public kernel::AgentKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeConverter( const dispatcher::Model_ABC& model );
    virtual ~AgentKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC* Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner );
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner );
    virtual const kernel::AgentKnowledge_ABC*      FindAgent( unsigned long id, const kernel::Entity_ABC& owner );
    virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long id, const kernel::Entity_ABC& owner );
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& base, const kernel::Entity_ABC& owner );
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& base, const kernel::Entity_ABC& owner );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgeConverter( const AgentKnowledgeConverter& );            //!< Copy constructor
    AgentKnowledgeConverter& operator=( const AgentKnowledgeConverter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    const kernel::Entity_ABC& FindGroup( const kernel::Entity_ABC& owner );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};

}

#endif // __AgentKnowledgeConverter_h_
