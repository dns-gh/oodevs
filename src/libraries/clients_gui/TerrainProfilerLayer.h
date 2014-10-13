// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TerrainProfilerLayer_h_
#define __TerrainProfilerLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  TerrainProfilerLayer
    @brief  TerrainProfilerLayer
*/
// Created: SBO 2010-03-31
// =============================================================================
class TerrainProfilerLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainProfilerLayer( kernel::Controllers& controllers, GLView_ABC& tools );
    virtual ~TerrainProfilerLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    void SetFromPosition( const geometry::Point2f& point );
    void SetToPosition( const geometry::Point2f& point );
    void SetCurrentPosition( const geometry::Point2f& point );
    void ClearCurrentPosition();
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f from_;
    geometry::Point2f to_;
    geometry::Point2f current_;
    //@}
};

}

#endif // __TerrainProfilerLayer_h_
