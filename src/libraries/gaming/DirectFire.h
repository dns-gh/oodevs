// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DirectFire_h_
#define __DirectFire_h_

#include "Fire_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Population_ABC;
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
                         const tools::Resolver_ABC< kernel::Agent_ABC >& agentResolver,
                         const tools::Resolver_ABC< kernel::Population_ABC >& populationResolver,
                         unsigned long entityId );
    virtual ~DirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

public:
    //! @name Member data
    //@{
    unsigned int id_;
    const kernel::Positions* position_;
    bool isTarget_;
    //@}
};

#endif // __DirectFire_h_
