// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Elevation3dLayer.h"

#include "GLTools_ABC.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/ModelLoaded.h"
#include "tools/ExerciseConfig.h"

#include <graphics/CompiledVisitor3d.h>
#include <graphics/ElevationFactory.h>
#include <graphics/extensions.h>
#include <graphics/FragmentShader.h>
#include <graphics/Lighting_ABC.h>
#include <graphics/MultiTextureLayer.h>
#include <graphics/ShaderProgram.h>
#include <graphics/TextureSet.h>
#include <graphics/Visitor3d.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation3dLayer::Elevation3dLayer( Controllers& controllers, GlTools_ABC& tools, const DetectionMap& elevation, Lighting_ABC& lighting )
    : Layer3D( controllers, tools, eLayerTypes_Elevation3d )
    , elevation_     ( elevation )
    , lighting_      ( lighting )
    , zRatio_        ( 5.f )
    , reset_         ( false )
    , ignoreShader_  ( false )
    , ignoreTextures_( false )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation3dLayer::~Elevation3dLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation3dLayer::NotifyUpdated( const ModelLoaded& modelLoaded )
{
    graphicsDirectory_.Clear();
    graphicsDirectory_ = modelLoaded.config_.GetGraphicsDirectory();
    reset_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::HandleKeyPress
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
bool Elevation3dLayer::HandleKeyPress( QKeyEvent* event )
{
    if( event  && event ->key() == Qt::Key_Plus )
    {
        zRatio_ *= 1.1f;
        return true;
    }
    if( event  && event ->key() == Qt::Key_Minus )
    {
        zRatio_ *= 0.9f;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation3dLayer::Paint( const ViewFrustum& frustum )
{
    if( !ShouldDrawPass() )
        return;
    if( reset_ )
    {
        Reset();
        reset_ = false;
    }

    if( !program_.get() && ! ignoreShader_ )
        CreateShaders();
    if( ! textures_.get() && !graphicsDirectory_.IsEmpty() && !ignoreTextures_ )
        CreateTextures();
    if( ! visitor_.get() && !graphicsDirectory_.IsEmpty() && elevation_.Data() )
        visitor_.reset( new CompiledVisitor3d( elevation_.GetMap() ) );

    if( !textures_.get() || !visitor_.get() )
        return;

    if( frustum != lastFrustum_ )
    {
        glPushAttrib( GL_TEXTURE_BIT );
            textures_->Accept( visitor_->Compiler( frustum, 1 ) );
        glPopAttrib();
    }

    lighting_.Set();

    if( program_.get() )
    {
        program_->Use();
        program_->SetUniformValue( "tex0", 0 );
        program_->SetUniformValue( "tex1", 1 );
    }

    glPushAttrib( GL_CURRENT_BIT | GL_TEXTURE_BIT );
        glPushMatrix();
            glScalef( 1.f, 1.f, zRatio_ );
            glColor3f( 1, 1, 1 );
            textures_->Accept( visitor_->Compiled( frustum ) );
        glPopMatrix();
    glPopAttrib();

    if( program_.get() )
        program_->Unuse();
    glDisable( GL_LIGHTING );

    if( gl::HasMultiTexturing() )
    {
        gl::glActiveTexture( gl::GL_TEXTURE1 );
        glDisable( GL_TEXTURE_2D );
        gl::glActiveTexture( gl::GL_TEXTURE0 );
    }

    lastFrustum_ = frustum;
}

namespace
{
    static const char* shagger =
    "uniform sampler2D tex0;"
    "uniform sampler2D tex1;"
    "void main()"
    "{"
    "   vec4 color    = texture2D(tex0,gl_TexCoord[0].st);"
    "   vec3 normal   = 2.0 * ( texture2D(tex1,gl_TexCoord[1].st).xyz - vec3( 0.5, 0.5, 0.5 ) );"
    "   normal        = normalize( gl_NormalMatrix * normal );"
    "   vec3 lightDir = normalize( vec3( gl_LightSource[0].position ) );"
    "   float diffuse = max( dot(normal, lightDir), 0.0 );"
    "   gl_FragColor  = color * ( gl_LightSource[0].ambient + gl_LightSource[0].diffuse * diffuse );"
    "   gl_FragColor.w = 1.0;"
    "}";
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::CreateShaders
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Elevation3dLayer::CreateShaders()
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
// Name: Elevation3dLayer::CreateTextures
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void Elevation3dLayer::CreateTextures()
{
    try
    {
        textures_.reset( new MultiTextureLayer() );
        usrp_.   reset( new TextureSet( graphicsDirectory_ / "usrp.texture" ) );
        if( ! ignoreShader_ )
            normals_.reset( new TextureSet( graphicsDirectory_ / "normals.texture" ) );
        textures_->SetLayer( 0, *usrp_ );
        if( ! ignoreShader_ )
            textures_->SetLayer( 1, *normals_ );
    }
    catch( ... )
    {
         ignoreTextures_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation3dLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Elevation3dLayer::Reset()
{
    lastFrustum_ = ViewFrustum();
    usrp_.reset();
    normals_.reset();
    textures_.reset();
    fragment_.reset();
    program_.reset();
    visitor_.reset();
    zRatio_ = 5.f;
    ignoreTextures_ = ignoreShader_ = false;
}
