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

#include "ASN_Types.h"
#include "Fire_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Population_ABC;
    class Agent_ABC;
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
             DirectFire( const ASN1T_MsgStartPionFire& message, 
                 const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, 
                 const kernel::Resolver_ABC< kernel::Population_ABC >& populationResolver );
    virtual ~DirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

public:
    //! @name Member data
    //@{
    unsigned int id_;
    const kernel::Entity_ABC* target_;
    //@}
};

#endif // __DirectFire_h_
