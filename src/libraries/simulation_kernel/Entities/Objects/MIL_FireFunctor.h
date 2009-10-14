// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//------------------------------------------------------------------------------
// Name: MIL_DynamicFire.cpp
// Created: RFT 22/04/2008
// Modified: RFT 06/05/2008
//------------------------------------------------------------------------------

#ifndef __MIL_FireFunctor_h_
#define __MIL_FireFunctor_h_

#include "MIL_FireClass.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"

class PHY_ComposantePion;
class PHY_Weapon;

class MIL_FireFunctor : public OnComponentFunctor_ABC
{
public:
    explicit MIL_FireFunctor( const MIL_FireClass& fireClass );
    ~MIL_FireFunctor();
    
    void operator()( PHY_ComposantePion& composantePion );
    void operator()( const PHY_ComposantePion& composantePion, const PHY_Weapon& weapon );

    //!@name Accessors
    //@{
    int GetNumberOfTheExtinguisherAgent();
    int GetFireHoseRange();
    //@}

private:
    const MIL_FireClass& fireClass_;
    MIL_FireClass::T_EvaluationResult bestExtinguisherAgentandFireHoseRange_;

};

#endif // __MIL_FireFunctor_h_

