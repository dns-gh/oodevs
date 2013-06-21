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
    class KnowledgeGroupFactory_ABC; // LTO
    class SymbolFactory;
}

namespace tools
{
    class IdManager;
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
    AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, tools::IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory, kernel::SymbolFactory& symbolsFactory );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC*      Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false, const QString& name = "" );
    virtual kernel::Automat_ABC*    Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name = "" );
    virtual kernel::Population_ABC* Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, int number, const geometry::Point2f& position );
    virtual kernel::Inhabitant_ABC* Create( kernel::Entity_ABC& parent, const kernel::InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location );

    virtual kernel::Agent_ABC*      Create( xml::xistream& xis, kernel::Automat_ABC& parent );
    virtual kernel::Automat_ABC*    Create( xml::xistream& xis, kernel::Entity_ABC& parent );
    virtual kernel::Population_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::PopulationType& type );
    virtual kernel::Inhabitant_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::InhabitantType& type );

    virtual kernel::Agent_ABC*   Create( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f position );
    virtual kernel::Automat_ABC* Create( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type );
    //@}

public:
    //! @name Helpers
    //@{
    static kernel::Entity_ABC* FindOrCreateKnowledgeGroup( const kernel::Entity_ABC& parent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    tools::IdManager& idManager_;
    kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_; // LTO
    kernel::SymbolFactory& symbolsFactory_;
    //@}
};

#endif // __AgentFactory_h_
