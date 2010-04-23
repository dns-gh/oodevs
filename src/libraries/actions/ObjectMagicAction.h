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
    class ModelVisitor_ABC;
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
             ObjectMagicAction( const kernel::Entity_ABC* object, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered = true );
             ObjectMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC* object );
    virtual ~ObjectMagicAction();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Polish();
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual void Publish( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long objectId_;
    kernel::Controller& controller_;
    bool registered_;
    //@}
};

}

#endif // __ObjectMagicAction_h_
