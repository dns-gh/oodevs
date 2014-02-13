// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UnitMagicAction_h_
#define __UnitMagicAction_h_

#include "Action_ABC.h"

namespace kernel
{
    class Controller;
    class MagicActionType;
}

namespace actions
{

// =============================================================================
/** @class  UnitMagicAction
    @brief  UnitMagicAction
*/
// Created: JSR 2010-04-02
// =============================================================================
class UnitMagicAction : public Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true);
    virtual ~UnitMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

}

#endif // __UnitMagicAction_h_
