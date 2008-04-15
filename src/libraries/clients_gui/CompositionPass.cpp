// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CompositionPass.h"
#include "TextureRenderPass.h"
#include "GlWidget.h"
#include "graphics/extensions.h"
#include "graphics/ShaderProgram.h"
#include "graphics/FragmentShader.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CompositionPass constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
CompositionPass::CompositionPass( TextureRenderPass& first, TextureRenderPass& second, kernel::Controllers& controllers, const std::string& option /*= ""*/ )
    : controllers_( controllers )
    , first_( first )
    , second_( second )
    , ignoreShader_( false )
    , option_( option )
    , enabled_( option_.empty() )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CompositionPass destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
CompositionPass::~CompositionPass()
{
    controllers_.Unregister( *this );
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
void CompositionPass::Render( GlWidget& )
{
    if( !enabled_ )
        return;
    glClear( GL_COLOR_BUFFER_BIT );
    if( !ignoreShader_ && !program_.get() )
        Initialize();
    if( program_.get() )
    {
        program_->Use();
        program_->SetUniformValue( "tex0", 0 );
        program_->SetUniformValue( "tex1", 1 );
        program_->SetUniformValue( "texelSize", 2.f / Width(), 2.f / Height(), 1 );
    }

    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
    first_.BindTexture();
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glEnable( GL_TEXTURE_2D );
    second_.BindTexture();
    glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 );
        glVertex2f( Viewport().Left(), Viewport().Bottom() );
        glTexCoord2f( 1, 0 );
        glVertex2f( Viewport().Right(), Viewport().Bottom() );
        glTexCoord2f( 1, 1 );
        glVertex2f( Viewport().Right(), Viewport().Top() );
        glTexCoord2f( 0, 1 );
        glVertex2f( Viewport().Left(), Viewport().Top() );
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );

    if( program_.get() )
        program_->Unuse();
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Width
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int CompositionPass::Width() const
{
    return first_.Width();
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Height
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
unsigned int CompositionPass::Height() const
{
    return first_.Height();
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Viewport
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
geometry::Rectangle2f CompositionPass::Viewport() const
{
    return first_.Viewport();
}

namespace
{
    static const char* shagger = 
    "uniform sampler2D tex0;"
    "uniform sampler2D tex1;"
    "uniform vec3 texelSize;"
    "void main()"
    "{"
    "   vec4 mainColor = texture2D( tex0, gl_TexCoord[0].st );"
    "   vec4 fogColor  = texture2D( tex1, gl_TexCoord[0].st );"
    "   vec4 blur      = texture2D( tex0, gl_TexCoord[0].st + texelSize.xy );"
    "   blur          += texture2D( tex0, gl_TexCoord[0].st - texelSize.xy );"
    "   blur          += texture2D( tex0, gl_TexCoord[0].st + vec2( texelSize.x, -texelSize.y ) );"
    "   blur          += texture2D( tex0, gl_TexCoord[0].st - vec2( texelSize.x, -texelSize.y ) );"
    "   blur          /= 4.;"
    "   vec4 color     = ( mainColor + 3. * fogColor.r * mainColor + ( 1. - fogColor.r ) * blur ) / 4.;"
    "   gl_FragColor   = color;"
    "}";
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::Initialize
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void CompositionPass::Initialize()
{
    gl::Initialize();
    try 
    {
        fragment_.reset( new gl::FragmentShader( shagger ) );
        program_.reset( new gl::ShaderProgram() );
        program_->AddShader( *fragment_ );
        program_->Link();
    }
    catch( ... )
    {
        ignoreShader_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: CompositionPass::OptionChanged
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void CompositionPass::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == option_ )
        enabled_ = value.To< bool >();
}
