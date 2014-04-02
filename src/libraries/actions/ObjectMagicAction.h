// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ObjectMagicAction_h_
#define __ObjectMagicAction_h_

#include "Action_ABC.h"

namespace kernel
{
    class Controller;
    class MagicActionType;
}

namespace actions
{

// =============================================================================
/** @class  ObjectMagicAction
    @brief  ObjectMagicAction
*/
// Created: JSR 2010-04-02
// =============================================================================
class ObjectMagicAction : public Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectMagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true );
    virtual ~ObjectMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Publish( Publisher_ABC& publisher, int context ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

}

#endif // __ObjectMagicAction_h_
