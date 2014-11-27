// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_SelectionLayer_h_
#define __gui_SelectionLayer_h_

#include "Layer.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  SelectionLayer
    @brief  SelectionLayer
*/
// Created: JSR 2012-05-23
// =============================================================================
class SelectionLayer : public Layer
                     , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             SelectionLayer( kernel::Controllers& controllers,
                             GLView_ABC& tools,
                             ColorStrategy_ABC& strategy );
    virtual ~SelectionLayer();
    //@}

    //! @name Layer
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    virtual void Reset();
    //@}

private:
    //! @name SelectionObserver
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    ColorStrategy_ABC& strategy_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    T_Layer layer_;
    //@}
};

} //! namespace gui

#endif // __gui_SelectionLayer_h_
