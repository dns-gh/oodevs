// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/ConvoySupply/PHY_ActionLogistic.h $
// $Author: Nld $
// $Modtime: 10/02/05 10:50 $
// $Revision: 1 $
// $Workfile: PHY_ActionLogistic.h $
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
    MT_COPYNOTALLOWED( PHY_ActionLogistic )

public:
    typedef MIL_AgentPion ActorType;

public:
    explicit PHY_ActionLogistic( T& entity );
    virtual ~PHY_ActionLogistic();

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    //@}

private:
    T& entity_;
};

#include "PHY_ActionLogistic.inl"

#endif // __PHY_ActionLogistic_h_
