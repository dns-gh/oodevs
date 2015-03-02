// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RectangleSelectionLayer_h_
#define __gui_RectangleSelectionLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
// =============================================================================
/** @class  RectangleSelectionLayer
    @brief  RectangleSelectionLayer
*/
// Created: JSR 2012-05-23
// =============================================================================
class RectangleSelectionLayer : public Layer2D
{
public:
    //! @name Constructors/Destructor
    //@{
             RectangleSelectionLayer( kernel::Controllers& controllers, GLView_ABC& tools );
    virtual ~RectangleSelectionLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseRelease( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove ( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    bool displaying_;
    bool firstPointSet_;
    boost::optional< geometry::Point2f > topLeft_;
    boost::optional< geometry::Point2f > bottomRight_;
    //@}
};

}

#endif // __gui_RectangleSelectionLayer_h_
