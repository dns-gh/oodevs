// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_ABC_h_
#define __ActionFactory_ABC_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class Entity_ABC;
    class Mission;
    class FragOrder;
}

class Action_ABC;

// =============================================================================
/** @class  ActionFactory_ABC
    @brief  ActionFactory_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory_ABC() {}
    virtual ~ActionFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::Mission& mission ) const = 0;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrder& fragOrder ) const = 0;

    virtual Action_ABC* CreateAction( const ASN1T_MsgPionOrder& message ) const = 0;
    virtual Action_ABC* CreateAction( const ASN1T_MsgAutomateOrder& message ) const = 0;
    //@}
};

#endif // __ActionFactory_ABC_h_
