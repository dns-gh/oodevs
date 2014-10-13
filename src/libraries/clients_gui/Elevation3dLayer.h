// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Elevation3dLayer_h_
#define __Elevation3dLayer_h_

#include "Layer.h"
#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>
#include <graphics/ViewFrustum.h>

class MultiTextureLayer;
class Lighting_ABC;
class TextureSet;
class CompiledVisitor3d;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}
namespace gl
{
    class FragmentShader;
    class ShaderProgram;
}

namespace gui
{

// =============================================================================
/** @class  Elevation3dLayer
    @brief  Elevation3dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation3dLayer : public Layer3D
                       , public tools::ElementObserver_ABC< kernel::ModelLoaded >

{
public:
    //! @name Constructors/Destructor
    //@{
             Elevation3dLayer( kernel::Controllers& controllers,
                               GLView_ABC& tools,
                               const kernel::DetectionMap& elevation,
                               Lighting_ABC& lighting );
    virtual ~Elevation3dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateShaders();
    void CreateTextures();
    virtual bool HandleKeyPress( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& elevation_;
    Lighting_ABC& lighting_;
    ViewFrustum lastFrustum_;
    std::unique_ptr< TextureSet >         usrp_;
    std::unique_ptr< TextureSet >         normals_;
    std::unique_ptr< MultiTextureLayer >  textures_;
    std::unique_ptr< gl::FragmentShader > fragment_;
    std::unique_ptr< gl::ShaderProgram >  program_;
    std::unique_ptr< CompiledVisitor3d >  visitor_;
    float zRatio_;
    bool reset_;
    bool ignoreShader_;
    bool ignoreTextures_;
    tools::Path graphicsDirectory_;
    //@}
};

}

#endif // __Elevation3dLayer_h_
