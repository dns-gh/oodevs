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
                         kernel::Controller& controller,
                         const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver,
                         const tools::Resolver_ABC< kernel::PopulationPart_ABC >& populationResolver,
                         unsigned long entityId );
    virtual ~DirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f position_;
    bool isTarget_;
    kernel::Controller& controller_;
    //@}
};

#endif // __DirectFire_h_
