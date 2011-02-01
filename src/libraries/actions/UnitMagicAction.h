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

#include "ActionWithTarget_ABC.h"

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
class UnitMagicAction : public ActionWithTarget_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, const QString& name, bool registered = true);
             UnitMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity, const QString& name );
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
