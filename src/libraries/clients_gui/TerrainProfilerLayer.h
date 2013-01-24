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

#include "Layer_ABC.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
// =============================================================================
/** @class  TerrainProfilerLayer
    @brief  TerrainProfilerLayer
*/
// Created: SBO 2010-03-31
// =============================================================================
class TerrainProfilerLayer : public Layer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainProfilerLayer( const kernel::GlTools_ABC& tools );
    virtual ~TerrainProfilerLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    void SetFromPosition( const geometry::Point2f& point );
    void SetToPosition( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    geometry::Point2f from_;
    geometry::Point2f to_;
    //@}
};

}

#endif // __TerrainProfilerLayer_h_
