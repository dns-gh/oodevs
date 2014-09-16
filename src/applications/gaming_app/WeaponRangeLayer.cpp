// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeaponRangeLayer.h"
#include "gaming/Weapons.h"
#include "clients_gui/Viewport2d.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Options.h"
#include "MT_Tools/MT_Logger.h"
#include <graphics/extensions.h>
#include <graphics/VertexShader.h>
#include <graphics/FragmentShader.h>
#include <graphics/ShaderProgram.h>

WeaponRangeLayer::WeaponRangeLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                    gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayerBase( controllers, tools, strategy, view, profile, eLayerTypes_WeaponRanges )
    , controllers_( controllers )
    , strategy_( strategy )
    , ignoreShader_( false )
    , useColor_( false )
{
    controllers_.Register( *this );
}

WeaponRangeLayer::~WeaponRangeLayer()
{
    controllers_.Unregister( *this );
}

void WeaponRangeLayer::NotifyCreated( const kernel::Agent_ABC& entity )
{
    AddEntity( entity );
}

void WeaponRangeLayer::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    RemoveEntity( entity );
}

void WeaponRangeLayer::Paint( const geometry::Rectangle2f& extent )
{
    if( ignoreShader_ )
        return;
    if( !ShouldDrawPass() )
        return;
    auto ranges = ranges_;
    auto positions = positions_;
    ranges_.clear();
    positions_.clear();
    gui::Viewport2d viewport( extent );
    EntityLayerBase::Paint( viewport );
    if( ranges != ranges_ )
        Reset();
    if( !program_ )
        return;
    program_->Use();
    if( positions != positions_ )
        Update();
    glBegin( GL_QUADS );
    glVertex2f( extent.Left(), extent.Bottom() );
    glVertex2f( extent.Left(), extent.Top() );
    glVertex2f( extent.Right(), extent.Top() );
    glVertex2f( extent.Right(), extent.Bottom() );
    glEnd();
    program_->Unuse();
}

void WeaponRangeLayer::Reset()
{
    vertex_.reset();
    fragment_.reset();
    program_.reset();
    if( ranges_.empty() )
        return;
    gl::Initialize();
    try
    {
        vertex_.reset( new gl::VertexShader(
            "varying vec4 position;"
            "void main()"
            "{"
            "  position = gl_ModelViewMatrix * gl_Vertex;"
            "  gl_Position = ftransform();"
            "}" ) );
        fragment_.reset( new gl::FragmentShader( MakeFragment() ) );
        program_.reset( new gl::ShaderProgram() );
        program_->AddShader( *vertex_ );
        program_->AddShader( *fragment_ );
        program_->Link();
    }
    catch( ... )
    {
        MT_LOG_WARNING_MSG( "unable to create shader in " << __FUNCTION__ );
        ignoreShader_ = true;
        fragment_.reset();
        vertex_.reset();
        program_.reset();
    }
}

void WeaponRangeLayer::Update() const
{
    int position = 0;
    for( auto it = positions_.begin(); it != positions_.end(); ++it )
    {
        ++position;
        std::stringstream s;
        s << "pos_" << position;
        program_->SetUniformValue(
            s.str(),
            static_cast< int >( it->second.size() ),
            &it->second.front().X() );
    }
}

std::string WeaponRangeLayer::MakeFragment() const
{
    std::stringstream shader;
    shader <<
        "varying vec4 position;"
        "const float alpha = 0.5;";
    // for each team 'team' define a constant for its color,
    // a constant array for the minimum weapon ranges and
    // another one for the maximum weapon ranges and
    // finally a uniform for the positions
    std::size_t team = 0;
    for( auto it = ranges_.begin(); it != ranges_.end(); ++it )
    {
        ++team;
        const QColor c( it->first );
        const auto& range = it->second;
        const std::size_t size = range.size();
        shader <<
            "const vec3 color_" << team
                << " = vec3(" << c.redF() << "," << c.greenF() << "," << c.blueF() << ");"
            "const uint min_" << team << "[" << size << "] = uint[" << size << "](";
        for( auto it2 = range.begin(); it2 != range.end(); ++it2 )
        {
            shader << it2->first;
            if( std::next( it2 ) != range.end() )
                shader << ",";
        }
        shader <<
            ");"
            "const uint max_" << team << "[" << size << "] = uint[" << size << "](";
        for( auto it2 = range.begin(); it2 != range.end(); ++it2 )
        {
            shader << it2->second;
            if( std::next( it2 ) != range.end() )
                shader << ",";
        }
        shader <<
            ");"
            "uniform vec2 pos_" << team << "[" << range.size() << "];";
    }
    shader <<
        "void main()"
        "{"
        "  vec3 color = vec3( 0, 0, 0 );"
        "  int count = 0;";
    team = 0;
    for( auto it = ranges_.begin(); it != ranges_.end(); ++it )
    {
        ++team;
        const QColor c( it->first );
        shader <<
            "for( int team = 0; team < " << it->second.size() << "; ++team )"
            "{"
            "  float d = distance(position.xy, pos_" << team << "[team]);"
            "  if( min_" << team << "[team] <= d && max_" << team << "[team] >= d )"
            "  {"
            "    color += color_" << team << ";"
            "    ++count;"
            "  }"
            "}";
    }
    shader <<
        "  if( count )"
        "  {"
        "    gl_FragColor.rgb = color / count;"
        "    gl_FragColor.a = alpha;"
        "  }"
        "}";
    return shader.str();
}

void WeaponRangeLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& /*viewport*/, bool /*pickingMode*/ )
{
    if( ShouldDisplay( entity ) && tools_.ShouldDisplay( type_ ) )
    {
        // SelectColor actually controls the result of ShouldDisplay
        strategy_.SelectColor( static_cast< const kernel::Agent_ABC& >( entity ) );
        const geometry::Point2f position = GetPosition( entity );
        if( tools_.ShouldDisplay( "WeaponRanges" ) )
            if( const Weapons* weapons = entity.Retrieve< Weapons >() )
            {
                weapons->DrawEfficientRange( position, tools_ );
                if( weapons->GetMaxRange() > 0 )
                {
                    const auto color = useColor_ ? color_ : strategy_.FindColor( entity );
                    ranges_[ color.name() ].push_back( std::make_pair( weapons->GetMinRange(), weapons->GetMaxRange() ) );
                    positions_[ color.name() ].push_back( position );
                }
            }
    }
}

void WeaponRangeLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "EfficientRangeUseCustomColor" )
        useColor_ = value.To< bool >();
    else if( name == "EfficientRangeCustomColor" )
    {
        color_.setNamedColor( value.To< QString >() );
        if( useColor_ )
        {
            ranges_.clear();
            positions_.clear();
        }
    }
}
