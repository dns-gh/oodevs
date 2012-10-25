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
#include <deque>
#include <boost/noncopyable.hpp>

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
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Explosions( kernel::Controller& controller, FireResultFactory& factory );
    virtual ~Explosions();
    //@}

public:
    //! @name Types
    //@{
    typedef std::deque< AgentFireResult* >  T_AgentFires;
    typedef T_AgentFires::const_iterator  CIT_AgentFires;

    typedef std::deque< PopulationFireResult* >  T_PopulationFires;
    typedef T_PopulationFires::const_iterator  CIT_PopulationFires;
    //@}

    //! @name Operations
    //@{
    const T_AgentFires& GetAgentExplosions() const;
    const T_PopulationFires& GetPopulationExplosions() const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void UpdateData( const T& message );
    virtual void DoUpdate( const sword::Explosion& message );
    virtual void DoUpdate( const sword::StopUnitFire& message );
    virtual void DoUpdate( const sword::StopCrowdFire& message );

    void Update( const sword::UnitFireDamages& message );
    void Update( const sword::CrowdFireDamages& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    FireResultFactory& factory_;

    T_AgentFires       agentExplosions_;
    T_PopulationFires  populationExplosions_;
    //@}
};

#endif // __Explosions_h_
