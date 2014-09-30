// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionsLayer_h_
#define __ActionsLayer_h_

#include "actions/Action_ABC.h"
#include "clients_gui/Layer.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    template< typename T > class SafePointer;
}

namespace gui
{
    class GlTools_ABC;
}

// =============================================================================
/** @class  ActionsLayer
    @brief  ActionsLayer
*/
// Created: SBO 2010-04-30
// =============================================================================
class ActionsLayer : public gui::Layer
                   , public tools::SelectionObserver< actions::Action_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools );
    virtual ~ActionsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( gui::Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const actions::Action_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< actions::Action_ABC > current_;
    //@}
};

#endif // __ActionsLayer_h_
