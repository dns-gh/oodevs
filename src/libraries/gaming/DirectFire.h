// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DirectFire_h_
#define __DirectFire_h_

#include "Fire_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Profile_ABC;
    class Entity_ABC;
    class PopulationPart_ABC;
    class Agent_ABC;
    class Positions;
}

namespace sword
{
    class StartUnitFire;
}

// =============================================================================
/** @class  DirectFire
    @brief  DirectFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class DirectFire : public Fire_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DirectFire( const sword::StartUnitFire& message,
                         kernel::Controller& controller, const kernel::Profile_ABC& profile,
                         const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver,
                         const tools::Resolver_ABC< kernel::PopulationPart_ABC >& populationResolver,
                         unsigned long entityId );
    virtual ~DirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    geometry::Point2f ComputePosition( const kernel::Entity_ABC& entity ) const;

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Profile_ABC& profile_;
    const kernel::Entity_ABC& target_;
    bool isTarget_;
    geometry::Point2f position_;
    //@}
};

#endif // __DirectFire_h_
