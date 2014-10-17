// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Elevation2dLayer_h_
#define __Elevation2dLayer_h_

#include "Layer.h"
#include "tools/ElementObserver_ABC.h"

class TextureSet;
class ElevationShader;

namespace kernel
{
    class DetectionMap;
    class Controller;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ElevationExtrema;

// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2D
                       , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                       , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             Elevation2dLayer( kernel::Controllers& controllers,
                               GLView_ABC& view,
                               const kernel::DetectionMap& elevation );
    virtual ~Elevation2dLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );

    void CreateLayer();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< ElevationExtrema > extrema_;
    std::unique_ptr< ElevationShader > shader_;
    std::unique_ptr< TextureSet > layer_;
    bool modelLoaded_;
    //@}
};

} //! namespace gui

#endif // __Elevation2dLayer_h_
