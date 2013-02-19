// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MapLayerProxy_h_
#define __MapLayerProxy_h_

#include "Layer.h"

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
    explicit MapLayerProxy( Layer& layer );
    virtual ~MapLayerProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize            ( const geometry::Rectangle2f& extent );
    virtual void Paint                 ( const ViewFrustum& frustum );
    virtual void Paint                 ( const geometry::Rectangle2f& viewport );
    virtual void Paint                 ( kernel::Viewport_ABC& viewport );

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    Layer& layer_;
    //@}
};

}

#endif // __MapLayerProxy_h_
