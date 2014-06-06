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

#include "Gradient.h"
#include "Layer.h"

#include <tools/ElementObserver_ABC.h>

class TextureSet;
class ElevationShader;

namespace kernel
{
    class DetectionMap;
    class Controller;
    class ModelLoaded;
}

namespace gui
{
    class ElevationExtrema;
}

namespace gui
{
// =============================================================================
/** @class  Elevation2dLayer
    @brief  Elevation2dLayer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Elevation2dLayer : public Layer2D
                       , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
public:
    //! @name Constructors/Destructor
    //@{
             Elevation2dLayer( kernel::Controller& controller, const kernel::DetectionMap& elevation );
    virtual ~Elevation2dLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );

    void SetGradient( const Gradient& gradient );
    void SetHillShadeDirection( int angle );
    void EnableVariableGradient( bool enable );
    void SetHillShadeStrength( float strength );

    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateShader();
    void CreateTextures();
    void SetGradient();
    void Cleanup();
    void SetElevations();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< ElevationExtrema > extrema_;
    std::unique_ptr< ElevationShader > shader_;
    std::unique_ptr< TextureSet > layer_;
    bool reset_;
    bool modelLoaded_;
    bool ignore_;
    Gradient gradient_;
    bool updateGradient_;
    unsigned gradientTexture_;
    float hsx_, hsy_;
    float hsStrength_;
    bool enabled_;
    short minElevation_, maxElevation_;
    geometry::Rectangle2f lastViewport_;
    //@}
};

}

#endif // __Elevation2dLayer_h_
