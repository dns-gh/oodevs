// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsModel_h_
#define __AgentsModel_h_

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/WorldParameters.h"
#include "clients_kernel/ModelLoaded.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Population_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class Team_ABC;
    class Controllers;
}

namespace xml
{
    class xistream;
}

class AgentFactory_ABC;
class LimitsModel;
class ModelChecker_ABC;

// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public tools::Resolver< kernel::Agent_ABC >
                  , public tools::Resolver< kernel::Automat_ABC >
                  , public tools::Resolver< kernel::Population_ABC >
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                  , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                  , public tools::ElementObserver_ABC< kernel::Population_ABC >
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsModel( kernel::Controllers& controllers, AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position );
    void CreateAutomat( xml::xistream& xis, kernel::Entity_ABC& parent, LimitsModel& limits );
    kernel::Automat_ABC& CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name = "" );

    kernel::Automat_ABC& GetAutomat( unsigned long id );
    kernel::Automat_ABC* FindAutomat( unsigned long id );

    kernel::Agent_ABC& CreateAgent( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false, const QString& name = "" );
    void CreateAgent( xml::xistream& xis, kernel::Automat_ABC& parent );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;
    std::string GetLoadingErrors() const;

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( kernel::Entity_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position );
    void CreatePopulation( xml::xistream& xis, kernel::Team_ABC& parent );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );

    void Purge();
    bool CheckValidity( ModelChecker_ABC& checker ) const;
    //@}

    //! @name Delayed initialization
    //@{
    void ReadLogistic( xml::xistream& xis );
    template< typename H >
    void ReadLogisticLink( xml::xistream& xis, const std::string& type, kernel::Automat_ABC& source, kernel::Entity_ABC& destination );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentsModel( const AgentsModel& );            //!< Copy constructor
    AgentsModel& operator=( const AgentsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Population_ABC& agent );
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );

    void ReadLogisticLink( xml::xistream& xis, kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    AgentFactory_ABC& agentFactory_;
    kernel::WorldParameters parameters_;
    std::string errors_;
    //@}
};

#endif // __AgentsModel_h_
