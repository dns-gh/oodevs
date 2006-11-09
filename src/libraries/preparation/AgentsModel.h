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
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Population_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class Formation_ABC;
    class Team_ABC;
    class Controllers;
}

namespace xml
{
    class xistream;
}

class AgentFactory_ABC;
class LimitsModel;

// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public kernel::Resolver< kernel::Agent_ABC >
                  , public kernel::Resolver< kernel::Automat_ABC >
                  , public kernel::Resolver< kernel::Population_ABC >
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
                  , public kernel::ElementObserver_ABC< kernel::Automat_ABC >
                  , public kernel::ElementObserver_ABC< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsModel( kernel::Controllers& controllers, AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAutomat( kernel::Formation_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position );
    void CreateAutomat( xml::xistream& xis, kernel::Formation_ABC& parent, LimitsModel& limits );
    kernel::Automat_ABC& GetAutomat( unsigned long id );
    kernel::Automat_ABC* FindAutomat( unsigned long id );

    void CreateAgent( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false );
    void CreateAgent( xml::xistream& xis, kernel::Automat_ABC& parent );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( kernel::Team_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position );
    void CreatePopulation( xml::xistream& xis, kernel::Team_ABC& parent );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );

    void Purge();
    //@}

    //! @name Delayed initialization
    //@{
    void ReadLogistic( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsModel( const AgentsModel& );            //!< Copy constructor
    AgentsModel& operator=( const AgentsModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Population_ABC& agent );

    void ReadLogisticLink( xml::xistream& xis, kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    AgentFactory_ABC& agentFactory_;
    //@}
};

#endif // __AgentsModel_h_
