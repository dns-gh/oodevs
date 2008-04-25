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

#include "game_asn/Simulation.h"

namespace kernel
{
    class Entity_ABC;
    class MissionType;
    class FragOrderType;
}

namespace xml
{
    class xistream;
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
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const = 0;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const = 0;

    virtual Action_ABC* CreateAction( const ASN1T_MsgUnitOrder& message ) const = 0;
    virtual Action_ABC* CreateAction( const ASN1T_MsgAutomatOrder& message ) const = 0;
    virtual Action_ABC* CreateAction( const ASN1T_MsgPopulationOrder& message ) const = 0;

    virtual Action_ABC* CreateAction( xml::xistream& xis ) const = 0;
    //@}
};

#endif // __ActionFactory_ABC_h_
