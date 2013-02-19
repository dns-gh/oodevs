// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_SelectionLayer_h_
#define __gui_SelectionLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
}

namespace gui
{
// =============================================================================
/** @class  SelectionLayer
    @brief  SelectionLayer
*/
// Created: JSR 2012-05-23
// =============================================================================
class SelectionLayer : public Layer2D
{
public:
    //! @name Constructors/Destructor
    //@{
             SelectionLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools );
    virtual ~SelectionLayer();
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
    virtual bool HandleMouseMove ( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    bool displaying_;
    bool firstPointSet_;
    geometry::Point2f topLeft_;
    geometry::Point2f bottomRight_;
    //@}
};

}

#endif // __gui_SelectionLayer_h_
