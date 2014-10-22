// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __WatershedLayer_h_
#define __WatershedLayer_h_

#include "Layer.h"
#include "tools/ElementObserver_ABC.h"

class TextureSet;
class ElevationShader;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  WatershedLayer
    @brief  WatershedLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class WatershedLayer : public Layer2D
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             WatershedLayer( kernel::Controllers& controllers,
                             GLView_ABC& view,
                             const kernel::DetectionMap& elevation );
    virtual ~WatershedLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateLayer();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< ElevationShader > shader_;
    std::unique_ptr< TextureSet > layer_;
    bool modelLoaded_;
    //@}
};

} //! namespace gui

#endif // __WatershedLayer_h_
