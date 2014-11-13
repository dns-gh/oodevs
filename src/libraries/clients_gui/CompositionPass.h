// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompositionPass_h_
#define __CompositionPass_h_

#include "GlRenderPass_ABC.h"

namespace gl
{
    class FragmentShader;
    class ShaderProgram;
}

namespace gui
{
    class GLView_ABC;
    class TextureRenderPass;

// =============================================================================
/** @class  CompositionPass
    @brief  CompositionPass
*/
// Created: SBO 2008-04-14
// =============================================================================
class CompositionPass : public GlRenderPass_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CompositionPass( TextureRenderPass& first,
                              TextureRenderPass& second,
                              const GLView_ABC& view,
                              const std::string& option = "" );
    virtual ~CompositionPass();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const;
    virtual void Render( MapWidget_ABC& widget );
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    virtual geometry::Rectangle2f Viewport() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize();
    //@}

private:
    //! @name Member data
    //@{
    const GLView_ABC& view_;
    TextureRenderPass& first_;
    TextureRenderPass& second_;
    std::unique_ptr< gl::FragmentShader > fragment_;
    std::unique_ptr< gl::ShaderProgram > program_;
    unsigned noiseTexture_;
    bool ignoreShader_;
    std::string option_;
    float time_;
    //@}
};

} //! namespace gui

#endif // __CompositionPass_h_
