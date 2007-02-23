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

#include "game_asn/Asn.h"
#include "Fire_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class CoordinateConverter_ABC;
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
             IndirectFire( const ASN1T_MsgStartPionFire& message, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, const kernel::CoordinateConverter_ABC& converter );
    virtual ~IndirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    geometry::Point2f target_;
    //@}
};

#endif // __IndirectFire_h_
