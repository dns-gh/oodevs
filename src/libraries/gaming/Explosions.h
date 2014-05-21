// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Explosions_h_
#define __Explosions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <boost/ptr_container/ptr_deque.hpp>
#pragma warning( pop )

namespace sword
{
    class Explosion;
    class StopUnitFire;
    class StopCrowdFire;
    class UnitFireDamages;
    class CrowdFireDamages;
}

namespace kernel
{
    class Agent_ABC;
    class Controller;
}

class FireResultFactory;
class AgentFireResult;
class PopulationFireResult;

// =============================================================================
/** @class  Explosions
    @brief  Explosions
    // $$$$ AGE 2006-03-10: Aussi une extension d'agent. Renommer !
*/
// Created: AGE 2006-03-10
// =============================================================================
class Explosions : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::Explosion >
                 , public kernel::Updatable_ABC< sword::StopUnitFire >
                 , public kernel::Updatable_ABC< sword::StopCrowdFire >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< kernel::Agent_ABC >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Explosions( kernel::Controller& controller, FireResultFactory& factory );
    virtual ~Explosions();
    //@}

public:
    //! @name Operations
    //@{
    const boost::ptr_deque< AgentFireResult >& GetAgentExplosions() const;
    const boost::ptr_deque< PopulationFireResult >& GetPopulationExplosions() const;
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::Explosion& message );
    virtual void DoUpdate( const sword::StopUnitFire& message );
    virtual void DoUpdate( const sword::StopCrowdFire& message );

    void Update( const sword::UnitFireDamages& message, const kernel::Entity_ABC* firer, int id );
    void Update( const sword::CrowdFireDamages& message, const kernel::Entity_ABC* firer, int id );
    void Update( const kernel::Entity_ABC* target, const kernel::Entity_ABC* firer, int id );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    FireResultFactory& factory_;

    boost::ptr_deque< AgentFireResult > agentExplosions_;
    boost::ptr_deque< PopulationFireResult > populationExplosions_;
    //@}
};

#endif // __Explosions_h_
