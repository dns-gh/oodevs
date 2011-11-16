// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlToolsBase.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/FourStateOption.h"
#include "GLSymbols.h"
#include "SvglRenderer.h"
#include "GlTooltip.h"
#include "SvglProxy.h"
#include "TacticalGraphics.h"
#include <graphics/extensions.h>

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlToolsBase constructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::GlToolsBase( Controllers& controllers )
    : controllers_( controllers )
    , selected_   ( false )
    , renderer_   ( new SvglRenderer() )
    , symbols_    ( new GLSymbols( *renderer_ ) )
    , svgl_       ( new SvglProxy( *renderer_ ) )
    , graphics_   ( new TacticalGraphics( controllers ) )
    , billboard_  ( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase destructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::~GlToolsBase()
{
    controllers_.Unregister( *this );
    for( CIT_Icons it = icons_.begin(); it != icons_.end(); ++it )
        glDeleteTextures( 1, & it->second );
    glDeleteLists( billboard_, 1 );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::UnSelect
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
std::pair< bool, bool > GlToolsBase::UnSelect() const
{
    std::pair< bool, bool > result( selected_, superiorSelected_ );
    selected_ = superiorSelected_ = false;
    return result;
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Select
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlToolsBase::Select( bool b1, bool b2 ) const
{
    selected_ = b1;
    superiorSelected_ = b2;
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::ShouldDisplay
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
bool GlToolsBase::ShouldDisplay( const std::string& name /* = std::string()*/ ) const
{
    return ShouldDisplay( name, selected_, superiorSelected_ );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::ShouldDisplay
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
bool GlToolsBase::ShouldDisplay( const std::string& name, bool autoCondition ) const
{
    return ShouldDisplay( name, autoCondition, false );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::ShouldDisplay
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
bool GlToolsBase::ShouldDisplay( const std::string& name, bool b1, bool b2 ) const
{
    if( name.empty() )
        return b1;
    IT_Options it = options_.find( name );
    if( it == options_.end() )
    {
        const FourStateOption& option = controllers_.options_.GetOption( name, FourStateOption::Selected() ).To< FourStateOption >();
        it = options_.insert( std::make_pair( name, option ) ).first;
    }
    return it->second.IsSet( b1, b2 );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::BindIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::BindIcon( const char** xpm )
{
    unsigned int& texture = icons_[ xpm ];
    if( ! texture )
    {
        QImage image( xpm );
        if( ! image.isNull() )
        {
            image = image.convertDepth( 32 );
            int nTrueWidth = image.width();
            int nTrueHeight = image.height();
            int nWidth = 2;
            int nHeight = 2;
            while( nWidth < nTrueWidth )
                nWidth *= 2;
            while( nHeight < nTrueHeight )
                nHeight *= 2;

            QImage resizedImage = image.copy( 0, 0, nWidth, nHeight );
            glGenTextures( 1, & texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            gl::glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        }
    }
    glBindTexture( GL_TEXTURE_2D, texture );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::SetCurrentColor
// Created: AGE 2007-05-25
// -----------------------------------------------------------------------------
void GlToolsBase::SetCurrentColor( float r, float g, float b, float a /* = 1*/ )
{
    glColor4f( r, g, b, a );
    renderer_->SetCurrentColor( r, g, b, a );
    graphics_->SetCurrentColor( r, g, b, a );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::DefaultStyle
// Created: AGE 2008-05-07
// -----------------------------------------------------------------------------
std::string GlToolsBase::DefaultStyle()
{
    return SvglRenderer::DefaultStyle();
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::PrintApp6
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/  )
{
    symbols_->PrintApp6( symbol, style, viewport, vWidth, vHeight );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::DrawSvg
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void GlToolsBase::DrawSvg( const std::string& filename, const geometry::Rectangle2f& viewport, unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/ )
{
    svgl_->Draw( filename, viewport, vWidth, vHeight );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::DrawTacticalGraphics
// Created: SBO 2009-05-29
// -----------------------------------------------------------------------------
void GlToolsBase::DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, bool overlined )
{
    graphics_->Draw( symbol, location, viewport, *this, overlined );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Base
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase& GlToolsBase::Base() const
{
    return const_cast< GlToolsBase& >( *this );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::DrawBillboardRect
// Created: AGE 2007-04-02
// -----------------------------------------------------------------------------
void GlToolsBase::DrawBillboardRect()
{
    if( ! billboard_ )
    {
        billboard_ = glGenLists( 1 );
        glNewList( billboard_, GL_COMPILE );
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 1.f );
            glVertex2f( -1, -1 );
            glTexCoord2f( 1.f, 1.f );
            glVertex2f( 1, -1 );
            glTexCoord2f( 1.f, 0.f );
            glVertex2f( 1, 1 );
            glTexCoord2f( 0.f, 0.f );
            glVertex2f( -1, 1 );
        glEnd();
        glEndList();
    }
    glCallList( billboard_ );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::OptionChanged
// Created: AGE 2007-04-20
// -----------------------------------------------------------------------------
void GlToolsBase::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    IT_Options it = options_.find( name );
    if( it != options_.end() )
        it->second = value.To< FourStateOption >();
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::CreateTooltip
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
std::auto_ptr< GlTooltip_ABC > GlToolsBase::CreateTooltip() const
{
    throw std::runtime_error( __FUNCTION__ );
}
