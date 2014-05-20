// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultFactory_h_
#define __FireResultFactory_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class UnitFireDamages;
    class CrowdFireDamages;
    class Explosion;
    class StopUnitFire;
    class StopCrowdFire;
}

class AgentFireResult;
class PopulationFireResult;
class Model;
class Simulation;

namespace kernel
{
    class Agent_ABC;
    class Population_ABC;
    class Profile_ABC;
}

// =============================================================================
/** @class  FireResultFactory
    @brief  FireResultFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultFactory : private::boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FireResultFactory( Model& model, const Simulation& simulation, const kernel::Profile_ABC& profile );
    virtual ~FireResultFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentFireResult*      CreateFireResult( const sword::UnitFireDamages& message, const kernel::Entity_ABC* firer, int id );
    virtual AgentFireResult*      CreateFireResult( const kernel::Agent_ABC* target, const kernel::Entity_ABC* firer, int id );
    virtual PopulationFireResult* CreateFireResult( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer, int id );
    virtual PopulationFireResult* CreateFireResult( const kernel::Population_ABC& target, const kernel::Entity_ABC* firer, int id );
    const kernel::Entity_ABC*     GetFirer( const sword::Explosion& message ) const;
    const kernel::Entity_ABC*     GetFirer( const sword::StopUnitFire& message ) const;
    const kernel::Entity_ABC*     GetFirer( const sword::StopCrowdFire& message ) const;
    const kernel::Entity_ABC*     GetTarget( const sword::Explosion& message ) const;
    const kernel::Entity_ABC*     GetTarget( const sword::StopUnitFire& message ) const;
    const kernel::Entity_ABC*     GetTarget( const sword::StopCrowdFire& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    const Simulation& simulation_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __FireResultFactory_h_
