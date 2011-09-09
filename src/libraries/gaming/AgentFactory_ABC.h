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

namespace sword
{
    class AutomatCreation;
    class UnitCreation;
    class CrowdCreation;
    class PopulationCreation;
}

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
    class Population_ABC;
    class Inhabitant_ABC;
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
    virtual kernel::Automat_ABC* Create( const sword::AutomatCreation& message ) = 0;
    virtual kernel::Agent_ABC* Create( const sword::UnitCreation& message ) = 0;
    virtual kernel::Population_ABC* Create( const sword::CrowdCreation& message ) = 0;
    virtual kernel::Inhabitant_ABC* Create( const sword::PopulationCreation& message ) = 0;
    //@}
};

#endif // __AgentFactory_ABC_h_
