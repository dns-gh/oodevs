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

namespace kernel
{
    class Agent_ABC;
    class Population_ABC;
    class AgentType;
    class AutomatType;
    class PopulationType;
    class Team_ABC;
    class Automat_ABC;
    class Entity_ABC;
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
class AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory_ABC() {};
    virtual ~AgentFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC* Create( kernel::Automat_ABC& parent, const kernel::AgentType& type, const geometry::Point2f& position, bool commandPost = false ) = 0;
    virtual kernel::Automat_ABC* Create( kernel::Entity_ABC& parent, const kernel::AutomatType& type  ) = 0;
    virtual kernel::Population_ABC* Create( kernel::Entity_ABC& parent, const kernel::PopulationType& type, const geometry::Point2f& position ) = 0;

    virtual kernel::Agent_ABC* Create( xml::xistream& xis, kernel::Automat_ABC& parent ) = 0;
    virtual kernel::Automat_ABC* Create( xml::xistream& xis, kernel::Entity_ABC& parent ) = 0;
    virtual kernel::Population_ABC* Create( xml::xistream& xis, kernel::Team_ABC& parent ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory_ABC( const AgentFactory_ABC& );            //!< Copy constructor
    AgentFactory_ABC& operator=( const AgentFactory_ABC& ); //!< Assignement operator
    //@}
};

#endif // __AgentFactory_ABC_h_
