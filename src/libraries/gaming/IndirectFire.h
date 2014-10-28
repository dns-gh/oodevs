// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndirectFire_h_
#define __IndirectFire_h_

#include "Fire_ABC.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class StartUnitFire;
    class StartUnitFireDetection;
}

namespace kernel
{
    class Agent_ABC;
    class CoordinateConverter_ABC;
    class DotationType;
    class StaticModel;
}

// =============================================================================
/** @class  IndirectFire
    @brief  IndirectFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class IndirectFire : public Fire_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             IndirectFire( const sword::StartUnitFire& message,
                           const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                           const kernel::StaticModel& model );
             IndirectFire( const sword::StartUnitFireDetection& message, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::CoordinateConverter_ABC& converter );
    virtual ~IndirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& view ) const;
    //@}

private:
    //! @name Member data
    //@{
    const geometry::Point2f target_;
    const kernel::DotationType* ammo_;
    //@}
};

#endif // __IndirectFire_h_
