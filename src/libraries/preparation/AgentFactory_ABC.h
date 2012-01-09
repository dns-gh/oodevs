// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_ABC_h_
#define __AgentFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class AgentType;
    class Automat_ABC;
    class AutomatType;
    class Entity_ABC;
    class Ghost_ABC;
    class Inhabitant_ABC;
    class InhabitantType;
    class Location_ABC;
    class Population_ABC;
    class PopulationType;
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AgentFactory_ABC
    @brief  AgentFactory_ABC
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory_ABC() {}
    virtual ~AgentFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC*      Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false, const QString& name = "" ) = 0;
    virtual kernel::Automat_ABC*    Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name = "" ) = 0;
    virtual kernel::Population_ABC* Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, int number, const geometry::Point2f& position ) = 0;
    virtual kernel::Inhabitant_ABC* Create( kernel::Entity_ABC& parent, const kernel::InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location ) = 0;

    virtual kernel::Agent_ABC*      Create( xml::xistream& xis, kernel::Automat_ABC& parent ) = 0;
    virtual kernel::Automat_ABC*    Create( xml::xistream& xis, kernel::Entity_ABC& parent ) = 0;
    virtual kernel::Population_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::PopulationType& type ) = 0;
    virtual kernel::Inhabitant_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent, const kernel::InhabitantType& type ) = 0;

    virtual kernel::Agent_ABC*   Create( kernel::Ghost_ABC& ghost, const kernel::AgentType& type, const geometry::Point2f position ) = 0;
    virtual kernel::Automat_ABC* Create( kernel::Ghost_ABC& ghost, const kernel::AutomatType& type ) = 0;
    //@}
};

#endif // __AgentFactory_ABC_h_
