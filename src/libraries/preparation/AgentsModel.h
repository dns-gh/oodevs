// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsModel_h_
#define __AgentsModel_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Population_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class KnowledgeGroup_ABC;
    class Team_ABC;
}

class AgentFactory_ABC;

// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public kernel::Resolver< kernel::Agent_ABC >
                  , public kernel::Resolver< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentsModel( AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAgent( const kernel::Agent_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position );
    void CreateAgent( const kernel::KnowledgeGroup_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( const kernel::Team_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );

    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsModel( const AgentsModel& );            //!< Copy constructor
    AgentsModel& operator=( const AgentsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    AgentFactory_ABC& agentFactory_;
    //@}
};

#endif // __AgentsModel_h_
