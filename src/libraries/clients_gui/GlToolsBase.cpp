// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "GlToolsBase.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/Options.h"
#include "astec_kernel/TristateOption.h"
#include "GlFont.h"

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: GlToolsBase constructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::GlToolsBase( Controllers& controllers )
    : controllers_( controllers )
    , selected_( false ) 
    , app6Font_( 0 )
    , app6OutlinedFont_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase destructor
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase::~GlToolsBase()
{
    delete app6Font_;
    delete app6OutlinedFont_;
    for( CIT_Icons it = icons_.begin(); it != icons_.end(); ++it )
        glDeleteTextures( 1, & it->second );
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
    return controllers_.options_.GetOption( name, TristateOption::auto_ )
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
// Name: GlToolsBase::InitializeIconLocations
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::InitializeIconLocations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::IconLocation
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
Point2f GlToolsBase::IconLocation( const char** xpm )
{
    if( iconLocations_.empty() )
        InitializeIconLocations();
    return iconLocations_[ xpm ];
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::PrintApp6
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::PrintApp6( const std::string symbol, bool outlined )
{
    GlFont*& font = outlined ? app6OutlinedFont_ : app6Font_;
    if( ! font )
        font = new GlFont( "Scipio", outlined );
    font->Print( symbol );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::GetSize
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
Vector2f GlToolsBase::GetSize( const std::string& symbol )
{
    if( ! app6Font_ )
        app6Font_ = new GlFont( "Scipio" );
    return app6Font_->GetTextSize( symbol );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Base
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase& GlToolsBase::Base() const
{
    return const_cast< GlToolsBase& >( *this );
}

    
