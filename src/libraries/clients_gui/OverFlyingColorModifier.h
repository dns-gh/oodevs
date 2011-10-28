// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __OverFlyingColorModifier_h_
#define __OverFlyingColorModifier_h_

#include "ColorModifier_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/OverFlyingObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  OverFlyingColorModifier
    @brief  OverFlyingColorModifier
*/
// Created: ABR 2011-10-27
// =============================================================================
class OverFlyingColorModifier : public ColorModifier_ABC
                              , public tools::Observer_ABC
                              , public kernel::OverFlyingObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OverFlyingColorModifier( kernel::Controllers& controllers );
    virtual ~OverFlyingColorModifier();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyOverFlown( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > overFlownEntity_;
    //@}
};

}

#endif // __OverFlyingColorModifier_h_
