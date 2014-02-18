// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "GLWidget.h"
#include "svgl/svgl.h"
#include "svgl/color.h"
#pragma warning(push, 0)
#include <QtGui/qmessagebox.h>
#include <QtGui/qcursor.h>
#include <QtGui/QEvent.h>
#pragma warning(pop)
#include <sstream>
#include <iostream>

// -----------------------------------------------------------------------------
// Name: GLWidget constructor
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
GLWidget::GLWidget( QWidget* parent, const std::string& filename )
    : QGLWidget( parent )
    , zoom_( 1e-2f )
    , ratio_( 1.f )
    , context_( new svg::RenderingContext() )
    , references_( new svg::References() )
    , root_ ( 0 )
    , color_( new svg::Color( "rgb(128,224,255)" ) )
    , compile_( true )
{
    context_->PushProperty( svg::RenderingContext_ABC::color, *color_ );

    ComputeViewport();
    setFocusPolicy( Qt::StrongFocus );
    setMouseTracking( true );
    setAcceptDrops( true );
    setFocus();

    Parse( filename );
}

// -----------------------------------------------------------------------------
// Name: GLWidget destructor
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
GLWidget::~GLWidget()
{
    context_->PopProperty( svg::RenderingContext_ABC::color );
    delete color_;
    delete references_;
    delete context_;
}

// -----------------------------------------------------------------------------
// Name: GLWidget::Parse
// Created: AGE 2006-08-21
// -----------------------------------------------------------------------------
void GLWidget::Parse( const std::string& filename )
{
    if( filename.empty() )
        return;
    try
    {
        setCursor( QCursor( Qt::WaitCursor ) );
        references_->Clear();
        delete root_; root_ = 0;
        SVGFactory parser( renderer_ );
        if( compile_ )
            root_ = parser.Compile( filename, *references_, 1.f );
        else
            root_ = parser.Parse( filename, *references_ );
        setCursor( QCursor( Qt::ArrowCursor ) );
        filename_ = filename;
        updateGL();
        parentWidget()->setWindowTitle( QString( "SVGL - %1 - %2" )
                                   .arg( filename_.c_str() )
                                   .arg( compile_ ? "Compilé" : "Non compilé" ) );
    }
    catch( std::exception& e )
    {
        setCursor( QCursor( Qt::ArrowCursor ) );
        QMessageBox::critical( this, ( "Could not load file '" + filename + "'" ).c_str(), e.what(), QMessageBox::Ok, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: GLWidget::keyPressEvent
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
void GLWidget::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_C )
        compile_ = !compile_;
    if( event->key() == Qt::Key_C || event->key() == Qt::Key_F5 )
        Parse( filename_ );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::initializeGL
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void GLWidget::initializeGL()
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
// Name: GLWidget::resizeGL
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void GLWidget::resizeGL( int w, int h )
{
    ratio_ = float( w ) / float( h );
    ComputeViewport();
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( topLeft_.X(), bottomRight_.X(), bottomRight_.Y(), topLeft_.Y(), -300, 300 );
}

namespace
{
    float symetryMatrix[4][4] = 
    { { 1, 0, 0, 0 },
      { 0, -1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 } };
}

// -----------------------------------------------------------------------------
// Name: GLWidget::paintGL
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLineWidth( 1.f );
    glColor3f( 1, 1, 1 );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glMultMatrixf( (const float*)symetryMatrix );
    if( root_ && references_ )
    {
        const geometry::BoundingBox bbox( topLeft_.X(), bottomRight_.Y(), bottomRight_.X(), topLeft_.Y() );
        context_->SetViewport( bbox, width(), height() );
        root_->Draw( *context_, *references_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GLWidget::mouseReleaseEvent
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void GLWidget::mouseReleaseEvent( QMouseEvent* pEvent )
{
    if( pEvent && pEvent->button() == Qt::MidButton )
    {
        center_ = RetrieveCoordinates( *pEvent );
        ComputeViewport();
        resizeGL( width(), height() );
        updateGL();
    }
}

// -----------------------------------------------------------------------------
// Name: GLWidget::wheelEvent
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void GLWidget::wheelEvent( QWheelEvent* pEvent )
{
    if( pEvent->delta() > 0 )
        zoom_ *= 1.2f;
    else
        zoom_ /= 1.2f;
    ComputeViewport();
    resizeGL( width(), height() );
    updateGL();
}

// -----------------------------------------------------------------------------
// Name: GLWidget::RetrieveCoordinates
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
geometry::Point GLWidget::RetrieveCoordinates( const QMouseEvent& event )
{
    const float rX = topLeft_.X() + event.x() * ( bottomRight_.X() - topLeft_.X() ) / width();
    const float rY = topLeft_.Y() - event.y() * ( topLeft_.Y() - bottomRight_.Y() ) / height();
    return geometry::Point( rX, rY );
}

// -----------------------------------------------------------------------------
// Name: GLWidget::ComputeViewport
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void GLWidget::ComputeViewport()
{
    const double rHalfHeight = 1 / zoom_;
    const double rHalfWidth  = rHalfHeight * ratio_;
    topLeft_.X()     = static_cast< float >( center_.X() - rHalfWidth );
    bottomRight_.Y() = static_cast< float >( center_.Y() - rHalfHeight );
    bottomRight_.X() = static_cast< float >( center_.X() + rHalfWidth );
    topLeft_.Y()     = static_cast< float >( center_.Y() + rHalfHeight );
}
