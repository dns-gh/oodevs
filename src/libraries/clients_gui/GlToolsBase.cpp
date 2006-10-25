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
#include "svgl/svgl.h"
#include "GlFont.h"

using namespace geometry;
using namespace kernel;
using namespace gui;
using namespace svg;

// -----------------------------------------------------------------------------
// Name: GlToolsBase constructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::GlToolsBase( Controllers& controllers )
    : controllers_( controllers )
    , selected_( false ) 
    , current_( new Color( "black" ) )
    , renderer_( new TextRenderer() )
    , references_( new References() )
    , renderingContext_( new RenderingContext() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase destructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::~GlToolsBase()
{
    for( CIT_Icons it = icons_.begin(); it != icons_.end(); ++it )
        glDeleteTextures( 1, & it->second );
    for( CIT_Symbols it = symbols_.begin(); it != symbols_.end(); ++it )
    {
        delete it->second.first;
        delete it->second.second;
    }
    delete current_;
    delete renderer_;
    delete references_;
    delete renderingContext_;
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
    return controllers_.options_.GetOption( name, TristateOption::Auto() )
        .To< TristateOption >().IsSet( autoCondition );
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

namespace
{
    void SetCurrentColor( Color& col )
    {
        // $$$$ AGE 2006-10-25: pas terrible
        float color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        col.Set( color[0], color[1], color[2] );
    }
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::PrintApp6
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport )
{
    // $$$$ AGE 2006-10-24: 
    const bool create = symbols_.find( symbol ) == symbols_.end();
    T_LodSymbol& node = symbols_[ symbol ];
    if( create )
    {
        // $$$$ AGE 2006-09-11: error management !
        SVGFactory factory( *renderer_ );
        try
        {
            node.first  = factory.Compile( symbol + ".svg", *references_, 10  ); // $$$$ AGE 2006-09-11: 
            node.second = factory.Compile( symbol + ".svg", *references_, 100 ); // $$$$ AGE 2006-09-11: 
        }
        catch( ... )
        {
            // $$$$ AGE 2006-10-23: 
        };
    }
    Node_ABC* renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    if( renderNode )
    {
        const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
        SetCurrentColor( *current_ ); // $$$$ AGE 2006-10-25: 
        renderingContext_->SetViewport( box, 640, 480 ); // $$$$ AGE 2006-09-11: 
        renderingContext_->PushProperty( RenderingContext::color, *current_ );
        renderNode->Draw( *renderingContext_, *references_ );
        renderingContext_->PopProperty( RenderingContext::color );
    }
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Base
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase& GlToolsBase::Base() const
{
    return const_cast< GlToolsBase& >( *this );
}
