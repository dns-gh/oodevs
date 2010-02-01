// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class KnowledgeGroupFactory_ABC; // LTO
}

class Model;
class StaticModel;
class IdManager;

// =============================================================================
/** @class  AgentFactory
    @brief  Agent_ABC factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC* Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false );
    virtual kernel::Automat_ABC* Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type );
    virtual kernel::Population_ABC* Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position );

    virtual kernel::Agent_ABC* Create( xml::xistream& xis, kernel::Automat_ABC& parent );
    virtual kernel::Automat_ABC* Create( xml::xistream& xis, kernel::Entity_ABC& parent );
    virtual kernel::Population_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    kernel::Entity_ABC* FindorCreateKnowledgeGroup( const kernel::Entity_ABC& parent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    IdManager& idManager_;
    kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_; // LTO
    //@}
};

#endif // __AgentFactory_h_
