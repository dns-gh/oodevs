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
    , references_( new References() )
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

#include <qpicture.h>
namespace
{
    void ShowSymbol( const std::string& filename )
    {
        QDialog* dialog = new QDialog( 0, "dialog", false );
        QVBoxLayout* layout = new QVBoxLayout( dialog );
        dialog->setMinimumSize( 320, 200 );
        
        QLabel* name = new QLabel( dialog, "name" );
        name->setText( filename.c_str() );
        layout->add( name );
        
        QLabel* label = new QLabel( dialog, "label" );
        QPicture picture;
        picture.load( filename.c_str(), "svg" );
        label->setPicture( picture );
        layout->add( label );

        dialog->show();

    };
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::PrintApp6
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlToolsBase::PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport )
{
    T_LodSymbol& node = symbols_[ symbol ];
    if( ! node.first )
    {
        // $$$$ AGE 2006-09-11: error management !
        SVGFactory factory;
        // $$$$ AGE 2006-09-11: 
        node.first  = factory.Compile( "symbols/" + symbol, *references_, 10  ); // $$$$ AGE 2006-09-11: 
        node.second = factory.Compile( "symbols/" + symbol, *references_, 100 ); // $$$$ AGE 2006-09-11: 
//        ShowSymbol( "symbols/" + symbol );
    }
    const BoundingBox box( viewport.Left(), viewport.Bottom(), viewport.Right(), viewport.Top() );
    RenderingContext context( box, 640, 480 ); // $$$$ AGE 2006-09-11: 
    Node_ABC* renderNode = viewport.Width() > 30000 ? node.second : node.first;  // $$$$ AGE 2006-09-11: hardcoded lod
    renderNode->Draw( context, *references_ );
}

// -----------------------------------------------------------------------------
// Name: GlToolsBase::Base
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
GlToolsBase& GlToolsBase::Base() const
{
    return const_cast< GlToolsBase& >( *this );
}

    
