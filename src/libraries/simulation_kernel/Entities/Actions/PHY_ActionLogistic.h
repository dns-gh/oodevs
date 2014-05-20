// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionLogistic_h_
#define __PHY_ActionLogistic_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionLogistic
// Created: JVT 2004-08-03
// =============================================================================
template< typename T >
class PHY_ActionLogistic : public PHY_Action_ABC
{
public:
    //! @name Typedef
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActionLogistic( T& entity );
    virtual ~PHY_ActionLogistic();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void Stop();
    //@}

private:
    T& entity_;
};

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic constructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::PHY_ActionLogistic( T& entity )
    : PHY_Action_ABC()
    , entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic destructor
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
PHY_ActionLogistic< T >::~PHY_ActionLogistic()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::Execute
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::Execute()
{
    entity_.UpdateLogistic();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::ExecuteSuspended
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::ExecuteSuspended()
{
    Execute();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLogistic::Stop
// Created: LDC 2010-03-11
// -----------------------------------------------------------------------------
template< typename T >
void PHY_ActionLogistic< T >::Stop()
{
    // NOTHING
}

#endif // __PHY_ActionLogistic_h_
