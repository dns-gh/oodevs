// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TextureRenderPass.h"
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/extensions.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TextureRenderPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
TextureRenderPass::TextureRenderPass( const std::string& name,
                                      const std::string& option /* = ""*/ )
    : LayersRenderPass( name, true )
    , texture_( 0 )
    , option_( option )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextureRenderPass destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
TextureRenderPass::~TextureRenderPass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextureRenderPass::Render
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void TextureRenderPass::Render( GLView_ABC& view )
{
    if( !option_.empty() && !view.GetCurrentOptions().Get( option_ ).To< bool >() )
        return;
    if( !texture_ )
        CreateTexture();
    LayersRenderPass::Render( view );
    glBindTexture( GL_TEXTURE_2D, texture_ );
    int maxGlTextureSize = 0;
    const auto viewport = view.GetViewport();
    int width = static_cast< int >( viewport.Width() );
    int height = static_cast< int >( viewport.Height() );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxGlTextureSize );
    if( maxGlTextureSize !=  0 && width > maxGlTextureSize )
        width = static_cast< unsigned short >( maxGlTextureSize );
    glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0 );
}

// -----------------------------------------------------------------------------
// Name: TextureRenderPass::CreateTexture
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void TextureRenderPass::CreateTexture()
{
    glGenTextures( 1, &texture_ );
    glBindTexture( GL_TEXTURE_2D, texture_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: TextureRenderPass::BindTexture
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void TextureRenderPass::BindTexture() const
{
    glBindTexture( GL_TEXTURE_2D, texture_ );
}
