// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RasterLayer_h_
#define __RasterLayer_h_

#include "Layer.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controller;
    class ModelLoaded;
}

class TextureSet;

namespace gui
{

// =============================================================================
/** @class  RasterLayer
    @brief  RasterLayer
*/
// Created: AGE 2007-01-04
// =============================================================================
class RasterLayer : public Layer2D
                  , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RasterLayer( kernel::Controller& controller );
             RasterLayer( kernel::Controller& controller, const tools::Path& texture );
    virtual ~RasterLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
    //@}

private:
    void GenerateTexture( const tools::Path& texture );

private:
    //! @name Member data
    //@{
    kernel::Controller&         controller_;
    std::auto_ptr< TextureSet > textures_;
    bool                        ignore_;
    tools::Path                 directory_;
    const tools::Path           texture_;
    //@}
};

}

#endif // __RasterLayer_h_
