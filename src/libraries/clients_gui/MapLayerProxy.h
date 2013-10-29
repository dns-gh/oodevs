// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MapLayerProxy_h_
#define __MapLayerProxy_h_

#include "Layer.h"

namespace kernel
{
    class Logger_ABC;
}

namespace gui
{
// =============================================================================
/** @class  MapLayerProxy
    @brief  MapLayer proxy. To prevent ownership
*/
// Created: AGE 2006-03-29
// =============================================================================
class MapLayerProxy : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
             MapLayerProxy( Layer& layer, kernel::Logger_ABC& logger );
    virtual ~MapLayerProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize            ( const geometry::Rectangle2f& extent );
    virtual void Paint                 ( const ViewFrustum& frustum );
    virtual void Paint                 ( const geometry::Rectangle2f& viewport );
    virtual void Paint                 ( Viewport_ABC& viewport );

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    Layer* layer_;
    kernel::Logger_ABC& logger_;
    //@}
};

}

#endif // __MapLayerProxy_h_
