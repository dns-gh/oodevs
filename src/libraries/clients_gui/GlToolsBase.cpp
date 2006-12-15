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
#include "GlFont.h"
#include "GLSymbols.h"

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
