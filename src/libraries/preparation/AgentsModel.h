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

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "tools/WorldParameters.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Inhabitant_ABC;
    class Population_ABC;
    class AgentType;
    class AutomatType;
    class InhabitantType;
    class PopulationType;
    class Team_ABC;
    class Controllers;
    class Location_ABC;
    class Ghost_ABC;
}

namespace xml
{
    class xistream;
}

class AgentFactory_ABC;
class Model;
class StaticModel;

// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public tools::Resolver< kernel::Agent_ABC >
                  , public tools::Resolver< kernel::Automat_ABC >
                  , public tools::Resolver< kernel::Population_ABC >
                  , public tools::Resolver< kernel::Inhabitant_ABC >
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                  , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                  , public tools::ElementObserver_ABC< kernel::Population_ABC >
                  , public tools::ElementObserver_ABC< kernel::Inhabitant_ABC >
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsModel( kernel::Controllers& controllers, AgentFactory_ABC& agentFactory, const StaticModel& staticModel );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position );
    void CreateAutomat( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type, const geometry::Point2f& position );
    void CreateAutomat( xml::xistream& xis, kernel::Entity_ABC& parent, Model& model );
    kernel::Automat_ABC& CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name = "" );

    kernel::Automat_ABC& GetAutomat( unsigned long id );
    kernel::Automat_ABC* FindAutomat( unsigned long id );

    kernel::Agent_ABC& CreateAgent( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false, const QString& name = "" );
    void CreateAgent( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f& position );
    void CreateAgent( xml::xistream& xis, kernel::Automat_ABC& parent, Model& model );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( kernel::Entity_ABC& parent, const kernel::PopulationType& type, int number, const geometry::Point2f& position );
    void CreatePopulation( xml::xistream& xis, kernel::Team_ABC& parent, Model& model );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );

    void CreateInhabitant( kernel::Entity_ABC& parent, const kernel::InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location );
    void CreateInhabitant( xml::xistream& xis, kernel::Team_ABC& parent, Model& model );
    kernel::Inhabitant_ABC& GetInhabitant( unsigned long id );
    kernel::Inhabitant_ABC* FindInhabitant( unsigned long id );

    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateAutomatChildren( kernel::Automat_ABC& automat, const kernel::AutomatType& type, const geometry::Point2f& position );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Population_ABC& agent );
    virtual void NotifyDeleted( const kernel::Inhabitant_ABC& agent );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&   controllers_;
    AgentFactory_ABC&      agentFactory_;
    const StaticModel&     staticModel_;
    tools::WorldParameters parameters_;
    //@}
};

#endif // __AgentsModel_h_
