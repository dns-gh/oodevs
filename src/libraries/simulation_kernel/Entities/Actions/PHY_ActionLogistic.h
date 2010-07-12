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

#include "PHY_ActionLogistic.inl"

#endif // __PHY_ActionLogistic_h_
