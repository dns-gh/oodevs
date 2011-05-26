// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SymbolWidget.h"
#include "moc_ADN_SymbolWidget.cpp"

#include "ADN_Project_Data.h"
#include "svgl/svgl.h"
#include "geometry/types.h"
#include "clients_gui/SvglRenderer.h"
#include "clients_gui/GLSymbols.h"
#include "clients_kernel/SymbolFactory.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget constructor
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
ADN_SymbolWidget::ADN_SymbolWidget( QWidget* parent )
    : QGLWidget( parent )
    , ADN_Gfx_ABC()
    , renderer_( new SvglRenderer() )
    , symbols_ ( new GLSymbols( *renderer_ ) )
    , symbolFactory_( 0 )
    , currentSymbol_()
    , viewPort_( 0, 0, 0, 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget destructor
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
ADN_SymbolWidget::~ADN_SymbolWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::initializeGL
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::initializeGL()
{
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    renderText( 0, 0, "" );
    glEnable( GL_LINE_SMOOTH );
    glEnableClientState( GL_VERTEX_ARRAY );
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::resizeGL
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::resizeGL( int w, int h )
{
    viewPort_.Set( 0.f, 0.f, (float)w, (float)h );
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, w, h, 0, -300, 300 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::paintGL
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLineWidth( 1.f );
    glColor3f( 1, 1, 1 );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    /* Test line
    glColor3f( 0.1f, 0.1f, 0.1f );
    glEnable( GL_LINE_SMOOTH );
    glBegin( GL_LINES );
        glVertex2f( topLeft_.X(), topLeft_.Y() );
        glVertex2f( bottomRight_.X(), bottomRight_.Y() );
    glEnd();
    */
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glEnable( GL_LINE_SMOOTH );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
            DisplaySymbol();
        glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::paintGL
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::DisplaySymbol() const
{
    if( currentSymbol_.empty() )
        return;

    const float svgWidth = 360;
    const float scaleRatio = viewPort_.Width() / svgWidth;

    const float svgDeltaX = -20;
    const float svgDeltaY = -30;

    glScalef( scaleRatio, scaleRatio, 1 );
    glTranslatef( svgDeltaX, svgDeltaY, 0.0f );

    symbols_->PrintApp6( currentSymbol_, SvglRenderer::DefaultStyle(), viewPort_, (int)viewPort_.Width(), (int)viewPort_.Height());
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::ComputeViewport
// Created: NLD 2010-12-01
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::OnNatureChanged( const QString& nature )
{
    currentSymbol_ = symbolFactory_->CreateSymbol( std::string( nature ) );
    std::replace( currentSymbol_.begin(), currentSymbol_.end(), '*', 'f' );
    updateGL();
    emit SymbolChanged( QString( currentSymbol_.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::IsAvailable
// Created: RPD 2011-04-08
// -----------------------------------------------------------------------------
bool ADN_SymbolWidget::IsAvailable( const std::string& symbol ) const
{
    return symbolFactory_->IsThisChainAvailable( symbol );
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::SetSymbolFactory
// Created: ABR 2011-05-26
// -----------------------------------------------------------------------------
void ADN_SymbolWidget::SetSymbolFactory( kernel::SymbolFactory& factory )
{
    symbolFactory_ = &factory;
}
