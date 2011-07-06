// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __EngageMagicAction_h_
#define __EngageMagicAction_h_

#include "ActionWithTarget_ABC.h"

namespace kernel
{
    class Controller;
    class MagicActionType;
}

namespace actions
{

// =============================================================================
/** @class  EngageMagicAction
    @brief  EngageMagicAction
*/
    // Created: FDS 2010-11-22
// =============================================================================
class EngageMagicAction : public ActionWithTarget_ABC
{
public:

//! @name Constructors/Destructor
    //@{
             EngageMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, const QString& name, const bool engaged, bool registered = true );
             EngageMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity, const QString& name, const bool engaged);
    virtual ~EngageMagicAction();
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
    const kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    bool registered_;
    bool engaged_;
    //@}

};

}

#endif // __EngageMagicAction_h_
