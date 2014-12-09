// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CompositionPass.h"
#include "TextureRenderPass.h"
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "tools/GeneralConfig.h"
#include <graphics/extensions.h>
#include <graphics/ShaderProgram.h>
#include <graphics/FragmentShader.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CompositionPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
CompositionPass::CompositionPass( TextureRenderPass& first,
                                  TextureRenderPass& second,
                                  const std::string& option /* = ""*/ )
    : first_( first )
    , second_( second )
    , noiseTexture_( 0 )
    , ignoreShader_( false )
    , option_( option )
    , time_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositionPass destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
CompositionPass::~CompositionPass()
{
    glDeleteTextures( 1, &noiseTexture_ );
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::GetName
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string CompositionPass::GetName() const
{
    return "composition";
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Render
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void CompositionPass::Render( GLView_ABC& view )
{
    if( !option_.empty() && !view.GetCurrentOptions().Get( option_ ).To< bool >() ||
        !gl::HasMultiTexturing() )
        return;

    glClear( GL_COLOR_BUFFER_BIT );
    if( !ignoreShader_ && !program_.get() )
        Initialize();

    const auto viewport = view.GetViewport();
    if( program_.get() )
    {
        program_->Use();
        program_->SetUniformValue( "mainTexture", 0 );
        program_->SetUniformValue( "fogTexture", 1 );
        program_->SetUniformValue( "noiseTexture", 2 );
        program_->SetUniformValue( "parameters", 20.f / view.GetWidth(), 20.f / view.GetHeight(), time_ += 0.01f );
    }

    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    first_.BindTexture();
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glEnable( GL_TEXTURE_2D );
    second_.BindTexture();
    gl::glActiveTexture( gl::GL_TEXTURE2 );
    glEnable( gl::GL_TEXTURE_3D );
    glBindTexture( gl::GL_TEXTURE_3D, noiseTexture_ );

    glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 );
        glVertex2f( viewport.Left(), viewport.Bottom() );
        glTexCoord2f( 1, 0 );
        glVertex2f( viewport.Right(), viewport.Bottom() );
        glTexCoord2f( 1, 1 );
        glVertex2f( viewport.Right(), viewport.Top() );
        glTexCoord2f( 0, 1 );
        glVertex2f( viewport.Left(), viewport.Top() );
    glEnd();

    glBindTexture( gl::GL_TEXTURE_3D, 0 );
    glDisable( gl::GL_TEXTURE_3D );
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );

    if( program_.get() )
        program_->Unuse();
}

namespace
{
    static const char* shagger =

    "uniform sampler2D mainTexture;"
    "uniform sampler2D fogTexture;"
    "uniform sampler3D noiseTexture;"
    "uniform vec3      parameters;"
    "void main()"
    "{"
    "   vec4 mainColor = texture2D( mainTexture, gl_TexCoord[0].st );"
    "   vec4 fogColor  = texture2D( fogTexture,  gl_TexCoord[0].st );"
    "   vec4 noise1    = texture3D( noiseTexture, vec3( gl_TexCoord[0].st * 10.0,       fract( parameters.z ) ) );"
    "   vec4 noise2    = texture3D( noiseTexture, vec3( gl_TexCoord[0].st * 13.0, 1.0 - fract( parameters.z ) ) );"
    "   vec2 displace  = vec2( ( noise1.x - 0.5 ) * parameters.x, ( noise2.x - 0.5 ) * parameters.y );"
    "   vec4 displaced = texture2D( mainTexture, gl_TexCoord[0].st + displace );"
    "   vec4 fogged    = displaced / 2.;"
    "   vec4 color     = mix( fogged, mainColor, fogColor.r );"
    "   gl_FragColor   = color;"
    "}";
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Initialize
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void CompositionPass::Initialize()
{
    try
    {
        fragment_.reset( new gl::FragmentShader( shagger ) );
        program_.reset( new gl::ShaderProgram() );
        program_->AddShader( *fragment_ );
        program_->Link();

        glGenTextures( 1, &noiseTexture_ );
        glBindTexture( gl::GL_TEXTURE_3D, noiseTexture_ );
        const unsigned size = 64 * 64 * 64;
        char buffer[ size ];
        ::memset( buffer, 0, size );
        tools::Ifstream input( tools::GeneralConfig::BuildResourceChildFile( "noise.dds" ) );
        input.seekg( 128, std::ios_base::cur );
        input.read( buffer, size );
        gl::glTexImage3D( gl::GL_TEXTURE_3D, 0, GL_RGB, 64, 64, 64, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer );
        glTexParameteri( gl::GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri( gl::GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    catch( ... )
    {
        ignoreShader_ = true;
    }
}
