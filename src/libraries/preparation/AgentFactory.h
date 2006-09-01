// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Model;
class StaticModel;

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
             AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC* Create( const kernel::Agent_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position );
    virtual kernel::Agent_ABC* Create( const kernel::KnowledgeGroup_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position );
    virtual kernel::Population_ABC* Create( const kernel::Team_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AttachExtensions( kernel::Entity_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __AgentFactory_h_
