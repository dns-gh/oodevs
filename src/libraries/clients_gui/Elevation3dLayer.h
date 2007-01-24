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

#include "Layer_ABC.h"
#include "graphics/ElevationColor_ABC.h"
#include "graphics/ViewFrustum.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/WorldParameters.h"

class MultiTextureLayer;
class Lighting_ABC;
class TextureSet;

namespace kernel
{
    class Controller;
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
class Elevation3dLayer : private kernel::WorldParameters
                       , public Layer3d_ABC
                       , public kernel::Observer_ABC
                       , public kernel::ElementObserver_ABC< kernel::ModelLoaded >

{

public:
    //! @name Constructors/Destructor
    //@{
             Elevation3dLayer( kernel::Controller& controller, const kernel::DetectionMap& elevation, Lighting_ABC& lighting );
    virtual ~Elevation3dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void Reset();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Elevation3dLayer( const Elevation3dLayer& );            //!< Copy constructor
    Elevation3dLayer& operator=( const Elevation3dLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateShaders();
    void CreateTextures();
    virtual bool HandleKeyPress( QKeyEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DetectionMap& elevation_;
    Lighting_ABC& lighting_;
    ViewFrustum lastFrustum_;
    std::auto_ptr< TextureSet >         usrp_;
    std::auto_ptr< TextureSet >         normals_;
    std::auto_ptr< MultiTextureLayer >  textures_;
    std::auto_ptr< gl::FragmentShader > fragment_;
    std::auto_ptr< gl::ShaderProgram >  program_;
    float zRatio_;
    bool ignoreShader_;
    bool ignoreTextures_;
    //@}
};

}

#endif // __Elevation3dLayer_h_
