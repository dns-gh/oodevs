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

#include "Layer_ABC.h"

namespace gui
{

// =============================================================================
/** @class  MapLayerProxy
    @brief  MapLayerProxy. To prevent ownership
*/
// Created: AGE 2006-03-29
// =============================================================================
class MapLayerProxy : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MapLayerProxy( Layer_ABC& layer );
    virtual ~MapLayerProxy();
    //@}

    //! @name Operations
    //@{
    virtual void Paint                 ( const ViewFrustum& frustum );
    virtual void Initialize            ( const geometry::Rectangle2f& extent );
    virtual void Paint                 ( const geometry::Rectangle2f& viewport );
    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MapLayerProxy( const MapLayerProxy& );            //!< Copy constructor
    MapLayerProxy& operator=( const MapLayerProxy& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Layer_ABC& layer_;
    //@}
};

}

#endif // __MapLayerProxy_h_
