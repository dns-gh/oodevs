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
    class Controllers;
    class ModelLoaded;
}

class TextureSet;

namespace gui
{
    class GlTools_ABC;

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
    explicit RasterLayer( kernel::Controllers& controllers, GlTools_ABC& tools );
             RasterLayer( kernel::Controllers& controllers,
                          GlTools_ABC& tools,
                          const tools::Path& texture,
                          const QString& userName );
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
    std::unique_ptr< TextureSet > textures_;
    bool                        ignore_;
    tools::Path                 directory_;
    const tools::Path           texture_;
    //@}
};

}

#endif // __RasterLayer_h_
