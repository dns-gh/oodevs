// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlToolsBase.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/TristateOption.h"
#include "GLSymbols.h"
#include "GlTooltip.h"

using namespace geometry;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlToolsBase constructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::GlToolsBase( Controllers& controllers )
    : controllers_( controllers )
    , selected_( false )
    , symbols_( new GLSymbols() )
    , billboard_( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase destructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::~GlToolsBase()
{
    controllers_.Remove( *this );
    for( CIT_Icons it = icons_.begin(); it != icons_.end(); ++it )
        glDeleteTextures( 1, & it->second );
    glDeleteLists( billboard_, 1 );
    delete symbols_;
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Select
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
bool GlToolsBase::Select( bool bDenis ) const
{
    bool result = selected_;
    selected_ = bDenis;
    return result;
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::ShouldDisplay
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
bool GlToolsBase::ShouldDisplay( const std::string& name ) const
{
    return ShouldDisplay( name, selected_ );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::ShouldDisplay
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
bool GlToolsBase::ShouldDisplay( const std::string& name, bool autoCondition ) const
{
    IT_Options it = options_.find( name );
    if( it == options_.end() )
    {
        const TristateOption& option = controllers_.options_.GetOption( name, TristateOption::Auto() ).To< TristateOption >();
        it = options_.insert( std::make_pair( name, option ) ).first;
    }
    return it->second.IsSet( autoCondition );
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
            glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
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
void GlToolsBase::SetCurrentColor( float r, float g, float b, float a /*= 1*/ )
{
    glColor4f( r, g, b, a );
    symbols_->SetCurrentColor( r, g, b, a );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::PrintApp6
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth /*= 640*/, unsigned vHeight /*= 480*/  )
{
    symbols_->PrintApp6( symbol, viewport, vWidth, vHeight );
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
        it->second = value.To< TristateOption >();
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::CreateTooltip
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
std::auto_ptr< GlTooltip_ABC > GlToolsBase::CreateTooltip() const
{
    throw std::runtime_error( __FUNCTION__ );
}
