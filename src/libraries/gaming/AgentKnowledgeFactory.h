// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeFactory_h_
#define __AgentKnowledgeFactory_h_

#include "AgentKnowledgeFactory_ABC.h"
#include "PopulationKnowledgeFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class Profile_ABC;
}

class Model;

// =============================================================================
/** @class  AgentKnowledgeFactory
    @brief  AgentKnowledgeFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledgeFactory : public AgentKnowledgeFactory_ABC
                            , public PopulationKnowledgeFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeFactory( kernel::Controllers& controllers, Model& model,
                 const kernel::CoordinateConverter_ABC& converter, const kernel::Profile_ABC& profile );
    virtual ~AgentKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::AgentKnowledge_ABC*      CreateAgentKnowledge     ( const kernel::KnowledgeGroup_ABC& group, const sword::UnitKnowledgeCreation& message );
    virtual kernel::PopulationKnowledge_ABC* CreatePopulationKnowledge( const kernel::KnowledgeGroup_ABC& group, const sword::CrowdKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgeFactory( const AgentKnowledgeFactory& );            //!< Copy constructor
    AgentKnowledgeFactory& operator=( const AgentKnowledgeFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __AgentKnowledgeFactory_h_
