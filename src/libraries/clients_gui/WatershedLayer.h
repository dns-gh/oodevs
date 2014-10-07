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
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

class TextureSet;
class ElevationShader;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{
    class GlTools_ABC;

// =============================================================================
/** @class  WatershedLayer
    @brief  WatershedLayer
*/
// Created: SBO 2010-03-23
// =============================================================================
class WatershedLayer : public Layer2D
                     , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                     , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             WatershedLayer( kernel::Controllers& controllers,
                             GlTools_ABC& tools,
                             const kernel::DetectionMap& elevation );
    virtual ~WatershedLayer();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void NotifyUpdated( const kernel::ModelLoaded& modelLoaded );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Reset();
    //@}

    //! @name Accessors
    //@{
    void SetHeight( unsigned short height );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateShader();
    void CreateTextures();
    void Cleanup();
    void SetGradient();
    void MakeGlTexture();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& elevation_;
    std::unique_ptr< ElevationShader > shader_;
    std::unique_ptr< TextureSet > layer_;
    bool reset_;
    bool modelLoaded_;
    bool ignore_;
    bool updateGradient_;
    unsigned gradientTexture_;
    unsigned short gradientSize_;
    unsigned short height_;
    bool enabled_;
    bool inverted_;
    QColor color_;
    //@}
};

}

#endif // __WatershedLayer_h_
