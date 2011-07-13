// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UnitSymbols_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Symbols_Data.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_gui/GLSymbols.h"
#include "clients_gui/GlTooltip.h"
#include "clients_gui/SvglRenderer.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_kernel/SymbolFactory.h"
#include "svgl/Color.h"
#include "svgl/Node_ABC.h"
#include "svgl/References.h"
#include "svgl/RenderingContext.h"
#include "svgl/TextRenderer.h"
#include "svgl/SVGFactory.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <qbitmap.h>
#include <qgl.h>
#include <xeumeuleu/streams/xistreamstream.hpp>
#include <zipstream/zipstream.h>


// -----------------------------------------------------------------------------
// ADN_UnitSymbols_Data::SymbolInfo
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::SymbolInfo
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::UnitSymbolInfo::UnitSymbolInfo( const std::string& name, const std::string& symbol )
    : strName_ ( name )
    , fileName_( symbol )
    , guiSvgRender_( new gui::SvglRenderer() )
    , symbols_( new gui::GLSymbols( *guiSvgRender_ ) )
{
    std::replace( fileName_.begin(), fileName_.end(), '*', 'f' );

    Initialize();
    Draw();
    glFlush();
    QImage image( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, 32 );
    glReadPixels( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();
    pixmap_ = new QPixmap( image.smoothScale( QSize( SYMBOL_PIXMAP_SIZE, SYMBOL_PIXMAP_SIZE ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::~SymbolInfo
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::UnitSymbolInfo::~UnitSymbolInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::GetNodeName
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
std::string ADN_UnitSymbols_Data::UnitSymbolInfo::GetNodeName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::GetItemName
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
std::string ADN_UnitSymbols_Data::UnitSymbolInfo::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::GetPixmap
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
const QPixmap& ADN_UnitSymbols_Data::UnitSymbolInfo::GetPixmap() const
{
    return *pixmap_;
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::GetCode
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
const std::string ADN_UnitSymbols_Data::UnitSymbolInfo::GetCode() const
{
    return template_->GetCode().ascii();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::GetGeometry
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
const std::string ADN_UnitSymbols_Data::UnitSymbolInfo::GetGeometry() const
{
    return template_->GetType().ascii();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::Initialize
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::UnitSymbolInfo::Initialize()
{
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnableClientState( GL_VERTEX_ARRAY );
    glLineWidth( 1.f );
    glColor3f( 1.f, 1.f, 1.f );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::SymbolInfo::Draw
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::UnitSymbolInfo::Draw()
{
    const float symbolSize = 400.0f;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        glPushMatrix();
            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            glMatrixMode( GL_MODELVIEW );
            glLoadIdentity();
            glViewport( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
            glOrtho( 0, symbolSize, 0, symbolSize, -300, 300 );

            glColor3f( 0.f, 0.f, 0.f );
            glBegin( GL_QUADS );
            glVertex2f( 0.f,        0.f );
            glVertex2f( 0.f,        symbolSize );
            glVertex2f( symbolSize, symbolSize );
            glVertex2f( symbolSize, 0.f );
            glEnd();
            glColor3f( 0.9f, 0.9f, 0.9f );
            glBegin( GL_QUADS );
            glVertex2f(              SYMBOL_BG_MARGIN,               SYMBOL_BG_MARGIN );
            glVertex2f(              SYMBOL_BG_MARGIN,  symbolSize - SYMBOL_BG_MARGIN );
            glVertex2f( symbolSize - SYMBOL_BG_MARGIN,  symbolSize - SYMBOL_BG_MARGIN );
            glVertex2f( symbolSize - SYMBOL_BG_MARGIN,               SYMBOL_BG_MARGIN );
            glEnd();

            geometry::Rectangle2f viewPort( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
            symbols_->PrintApp6( fileName_, gui::SvglRenderer::DefaultStyle(), viewPort, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
        glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// ADN_UnitSymbols_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data constructor
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::ADN_UnitSymbols_Data()
    : pSymbolFactory_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data destructor
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::~ADN_UnitSymbols_Data()
{
    for( IT_UnitSymbolInfoVector it = symbols_.begin(); it != symbols_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::Reset
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::Reset()
{
    symbols_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::GetSymbol
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::UnitSymbolInfo* const ADN_UnitSymbols_Data::GetSymbol() const
{
    if ( symbols_.empty() )
        return 0;
    return *symbols_.begin();
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::GetSymbols
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
ADN_UnitSymbols_Data::T_UnitSymbolInfoVector& ADN_UnitSymbols_Data::GetSymbols()
{
    return symbols_;
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::FilesNeeded
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::FilesNeeded( T_StringList& /*vFiles*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::Load
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::Load( const tools::Loader_ABC& /*fileLoader*/ )
{
    pSymbolFactory_ = &ADN_Workspace::GetWorkspace().GetSymbols().GetData().GetSymbolFactory();
    
    const std::string strUndefined = "undefined";
    symbols_.AddItem( new UnitSymbolInfo( strUndefined, pSymbolFactory_->CreateSymbol( strUndefined ) ) );
    const std::vector< std::string >& symbols = pSymbolFactory_->GetAvailableSymbols();
    for ( unsigned int i=0; i < symbols.size(); ++i )
    {
        std::string symbol = pSymbolFactory_->CreateSymbol( symbols[i] );
        if ( symbol.empty() )
            symbol = pSymbolFactory_->CreateSymbol( strUndefined );
        symbols_.AddItem( new UnitSymbolInfo( symbols[i], symbol ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::Save
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::Save()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::IsValidDatabase
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
bool ADN_UnitSymbols_Data::IsValidDatabase()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::ReadArchive
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::ReadArchive( xml::xistream& /*input*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_UnitSymbols_Data::WriteArchive
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
void ADN_UnitSymbols_Data::WriteArchive( xml::xostream& /*output*/ )
{
    // NOTHING
}