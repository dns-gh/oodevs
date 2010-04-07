// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UnitMagicActionTeleport_h_
#define __UnitMagicActionTeleport_h_

#include "UnitMagicAction.h"

namespace kernel
{
    class Entity_ABC;
    class MagicActionType;
    class Controller;
}

namespace xml
{
    class xistream;
}

namespace actions
{

// =============================================================================
/** @class  UnitMagicActionTeleport
    @brief  UnitMagicActionTeleport
*/
// Created: JSR 2010-04-06
// =============================================================================
class UnitMagicActionTeleport : public UnitMagicAction
{

public:
    //! @name Constructors/Destructor
    //@{
    UnitMagicActionTeleport( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity );
    virtual ~UnitMagicActionTeleport();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __UnitMagicActionTeleport_h_
