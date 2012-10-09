// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SideResolver_ABC_H_
#define plugins_hla_SideResolver_ABC_H_

#include "rpr/ForceIdentifier.h"

namespace dispatcher
{
    class Team_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  SideResolver_ABC
    @brief  SideResolver_ABC
*/
// Created: AHC 2012-09-07
// =============================================================================
class SideResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~SideResolver_ABC() {}
    //@}
    //! @name Operations
    //@}
    virtual unsigned long ResolveTeam( rpr::ForceIdentifier f ) const = 0;
    virtual rpr::ForceIdentifier ResolveForce( unsigned long ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_SideResolver_ABC_H_
