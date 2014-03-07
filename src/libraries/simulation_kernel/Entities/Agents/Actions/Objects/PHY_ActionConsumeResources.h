// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActionConsumeResources_h__
#define __PHY_ActionConsumeResources_h__

#include "Entities/Actions/PHY_Action_ABC.h"
#include <boost/shared_ptr.hpp>

class MIL_Entity_ABC;
class PHY_DotationCategory;

namespace dotation
{
    class PHY_RoleInterface_Dotations;
}

// PHY_ActionConsumeResources consumes resources at each tick until
// stopped or until it has consumed its target
class PHY_ActionConsumeResources : public PHY_Action_ABC
{
public:
    typedef MIL_Entity_ABC ActorType;
    // Consumes value (in %) dotations during duration (in seconds)
    // from entity and selected dotation category
    PHY_ActionConsumeResources( MIL_Entity_ABC& entity,
                                const PHY_DotationCategory* category,
                                double value, double duration,
                                unsigned tickDuration );
    virtual ~PHY_ActionConsumeResources();

    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void Stop();

private:
    dotation::PHY_RoleInterface_Dotations& dotations_;
    const PHY_DotationCategory& category_;
    int steps_; // number of times to apply offset
    const double offset_; // offset to consume per tick
};

#endif // __PHY_ActionConsumeResources_h__
