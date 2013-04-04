// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DefaultLayer_h_
#define __DefaultLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  DefaultLayer
    @brief  DefaultLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class DefaultLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultLayer( kernel::Controllers& controllers );
    virtual ~DefaultLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void FillContextMenu( QMouseEvent* mouse, kernel::ContextMenu& menu, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    geometry::Point2f point_;
    //@}
};

}

#endif // __DefaultLayer_h_
