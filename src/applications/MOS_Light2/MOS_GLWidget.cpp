// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-02-27 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GLWidget.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:31 $
// $Revision: 20 $
// $Workfile: MOS_GLWidget.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_GLWidget.h"
#include "moc_MOS_GLWidget.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"
#include "MOS_GLTool.h"
#include "MOS_MapMouseEvent.h"
#include "MOS_MapDropEvent.h"

#include "MT_GLDrawer.h"
#include "MT_GLFont.h"

#ifndef MOS_USE_INLINE
#   include "MOS_GLWidget.inl"
#endif

// $$$$ AGE 2005-03-16: ...
// Helper function used for the MT_GLDrawer
MT_Vector2D ScreenToGL( const MT_Vector2D& screenPos, QGLWidget& glWidget )
{
    return ((MOS_GLWidget&)glWidget).ScreenToGL( QPoint( screenPos.rX_, screenPos.rY_ ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget constructor
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
MOS_GLWidget::MOS_GLWidget( QWidget* pParent, QGLWidget* pShare /*= 0*/ )
    : QGLWidget     ( QGLFormat( StencilBuffer ), pParent, "", pShare )
    , vCenter_      ( MT_Vector2D ( 0.0, 0.0 ) )
    , viewport_     ( MOS_App::GetApp().GetWorld().GetRect() )
    , rZoom_        ( 0.5 )
    , nRulerState_  ( eNone )
    , bUpdating_( false )
{
    glInit();
    this->setFocusPolicy( QWidget::ClickFocus );
    this->setMouseTracking( true );
    this->setAcceptDrops( true );

    pGLDrawer_ = new MT_GLDrawer( *this, &(::ScreenToGL), MOS_MainWindow::GetMainWindow() ); // $$$$ AGE 2005-03-16: ??
    pGLDrawer_->SetDefaults( 1.5f, 20.0f );

    MOS_App::GetApp().GetWorld().Initialize();
    vCenter_ = MOS_App::GetApp().GetWorld().GetRect().GetCenter();

    pTimer_ = new QTimer( this );
    connect( pTimer_, SIGNAL(timeout()), SLOT(update()) );

    Update();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget destructor
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
MOS_GLWidget::~MOS_GLWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::initializeGL
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::initializeGL()
{
    glShadeModel( GL_FLAT );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glEnable( GL_STENCIL );
    glEnable( GL_TEXTURE_2D );

	glDisable( GL_DEPTH_TEST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    MT_GLFont::Font( "Arial", 11.0, ANSI_CHARSET, true, FW_BOLD );
    GFX_Tools::InitializeFonts();
    renderText( 0, 0, "" );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::resizeGL
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::resizeGL( int w, int h )
{
    CalculateViewport( w, h );
    glViewport(0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( viewport_.GetLeft(), viewport_.GetRight()
           , viewport_.GetBottom(), viewport_.GetTop()
           , -300, 300 );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::CalculateViewport
// Created: AGE 2005-03-16
// -----------------------------------------------------------------------------
void MOS_GLWidget::CalculateViewport( int w, int h )
{
    const MT_Float rRatio = MT_Float( w ) / MT_Float( h );
    MOS_World* pWorld = & MOS_App::GetApp().GetWorld(); // $$$$ AGE 2005-03-11: Singletons are evil
    const double rHalfHeight = pWorld ? 0.5 * pWorld->GetRect().GetWidth() / rZoom_ : 0;
    const double rHalfWidth  =  rHalfHeight * rRatio;
    viewport_.Set( MT_Vector2D( vCenter_.rX_ - rHalfWidth, vCenter_.rY_ - rHalfHeight )
                 , MT_Vector2D( vCenter_.rX_ + rHalfWidth, vCenter_.rY_ + rHalfHeight ) );
    rMetersPerPixel_ = viewport_.GetWidth() / MT_Float( w );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::Update
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::Update( unsigned int nWait )
{
    if( ! bUpdating_ )
    {
        pTimer_->stop();
        pTimer_->start( nWait, true );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::paintGL
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::paintGL()
{
    bUpdating_ = true;
    glClear( GL_COLOR_BUFFER_BIT );
	glClear( GL_DEPTH_BUFFER_BIT );
    glPushMatrix();

    MOS_GLTool::Draw( MOS_App::GetApp().GetWorld() );
    MOS_GLTool::DrawGrid( MOS_App::GetApp().GetWorld() );
    MOS_GLTool::Draw( viewport_, rMetersPerPixel_ );

    pGLDrawer_->Draw();

    // Ruler
    if( nRulerState_ == eExistant )
    {
        glLineWidth( 2 );
        glColor4d( 1.0, 1.0, 1.0, 1.0 );
        double rDist = vRulerStartPos_.Distance( vRulerEndPos_ );
        double rMeters = rDist;

        MT_Vector2D vPosText =  MT_Vector2D( ( vRulerStartPos_ + vRulerEndPos_ ) / 2. );
        MOS_GLTool::SimpleMode sm;
        MOS_GLTool::DrawLine( vRulerStartPos_, vRulerEndPos_, rMetersPerPixel_ * 25, true );
        MT_GLFont::Print( "Arial", vPosText, MT_FormatString( "  %5.2fm", rMeters  ), rMetersPerPixel_ * 25 );
    }

	glPopMatrix();
	glFlush();

    bUpdating_ = false;
    Update( 250 );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::event
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
bool MOS_GLWidget::event( QEvent* pEvent )
{
    bool bDealt = false;
    // If it's a mouse event...
    if( pEvent->type() == QEvent::MouseButtonPress   || pEvent->type() == QEvent::MouseMove ||
        pEvent->type() == QEvent::MouseButtonRelease || pEvent->type() == QEvent::MouseButtonDblClick )
    {
        // Translate it into a custom map event and send it.
        MOS_MapMouseEvent mapMouseEvent( *(QMouseEvent*)pEvent, ScreenToGL( *(QMouseEvent*)pEvent ), rMetersPerPixel_ );
        bDealt = QGLWidget::event( &mapMouseEvent );
    }
    // If it's a drop event...
    else if( pEvent->type() == QEvent::Drop )
    {
        // Translate it into a custom map event and send it.
        MOS_MapDropEvent mapDropEvent( *(QDropEvent*)pEvent, ScreenToGL( ((QDropEvent*)pEvent)->pos() ) );
        bDealt = QGLWidget::event( &mapDropEvent );
    }
    // If it's not a mouse event or drop event, use the standard event processing method.
    else
        bDealt = QGLWidget::event( pEvent );

    if( bDealt )
        Update();
    return bDealt;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::hideEvent
// Created: AGE 2005-05-16
// -----------------------------------------------------------------------------
void MOS_GLWidget::hideEvent( QHideEvent* )
{
    pGLDrawer_->OnHide();
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), this,   SLOT( OnCenterOnPoint( const MT_Vector2D& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::showEvent
// Created: AGE 2005-05-16
// -----------------------------------------------------------------------------
void MOS_GLWidget::showEvent( QShowEvent* )
{
    pGLDrawer_->OnShow();
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), this,   SLOT( OnCenterOnPoint( const MT_Vector2D& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::mousePressEvent
/** @param  pMouseEvent 
*/
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::mousePressEvent( QMouseEvent* pMouseEvent )
{
    if( pMouseEvent->stateAfter() == Qt::MidButton
        || ( pMouseEvent->button() == LeftButton && pMouseEvent->state() == ControlButton ) )
    {
        vGrabPos_ = MT_Vector2D( pMouseEvent->x(), pMouseEvent->y() );
    }

    if( pMouseEvent->button() == LeftButton && pMouseEvent->state() == NoButton )
    {
        nRulerState_ = eStarting;
        vRulerStartPos_ = ScreenToGL( *pMouseEvent );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::mouseMoveEvent
/** @param  pMouseEvent 
*/
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::mouseMoveEvent( QMouseEvent* pMouseEvent )
{
    MT_Vector2D vMapPos = ScreenToGL( *pMouseEvent );
    if(  pMouseEvent->state() == MidButton
      || ( pMouseEvent->state() == (LeftButton | ControlButton) ) )
    {
        // Compute the screen displacement (invert Y coords).
        MT_Vector2D posMouse = MT_Vector2D( pMouseEvent->x(), pMouseEvent->y() );
        MT_Vector2D vDisplacement = vGrabPos_ - posMouse;

        // Compute the GL displacement length.
        MT_Vector2D vGLDisplacement = ScreenToGL( vDisplacement );
        MT_Vector2D vGLZero = ScreenToGL( MT_Vector2D(0.0,0.0) );
        
        
        vGLDisplacement = vGLDisplacement - vGLZero;

        vCenter_ = vCenter_ + vGLDisplacement;
        vGrabPos_ = posMouse;
        resizeGL( width(), height() );
        Update();
    }

    if( pMouseEvent->state() == LeftButton  )
    {
        if( nRulerState_ == eStarting )
        {
            if( vRulerStartPos_.Distance( vMapPos ) > 10 * rMetersPerPixel_ )
                nRulerState_ = eExistant;
        }
        if( nRulerState_ == eExistant )
        {
            vRulerEndPos_ = vMapPos;
            Update();
        }
    }

    emit MouseMove( pMouseEvent, vMapPos );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::mouseReleaseEvent
/** @param  pMouseEvent 
*/
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
void MOS_GLWidget::mouseReleaseEvent( QMouseEvent* /*pMouseEvent*/ )
{
    nRulerState_ = eNone;
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::wheelEvent
/** @param  pWheelEvent 
*/
// Created: APE 2004-03-01
// -----------------------------------------------------------------------------
void MOS_GLWidget::wheelEvent( QWheelEvent* pWheelEvent )
{
    MT_Vector2D glCoord = ScreenToGL( pWheelEvent->pos() );

    float rZoom;
    if( pWheelEvent->delta() > 0 )
        rZoom = 1.1f;
    else 
        rZoom = 0.9f;
//    vCenter_ += ( glCoord - vCenter_ ) * (double)( 1 - 1 / rZoom );
    rZoom_ *= rZoom;
    resizeGL( width(), height());
    Update();

    pWheelEvent->accept();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::keyPressEvent
/** @param  pKeyEvent 
*/
// Created: APE 2004-03-25
// -----------------------------------------------------------------------------
void MOS_GLWidget::keyPressEvent( QKeyEvent* pKeyEvent )
{
    switch( pKeyEvent->key() )
    {
        case Qt::Key_Plus:
        {
            rZoom_ *= 1.2;
            resizeGL( width(), height() );
            Update();
            return;
        }
        case Qt::Key_Minus:
        {
            rZoom_ *= 0.8;
            resizeGL( width(), height());
            Update();
            return;
        }
    }
    pKeyEvent->ignore();
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::OnCenterOnPoint
/** @param  vPoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
void MOS_GLWidget::OnCenterOnPoint( const MT_Vector2D& vPoint )
{
    vCenter_ = vPoint;
    resizeGL( width(), height());
    Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::ScreenToGL
// Created: AGE 2005-03-16
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GLWidget::ScreenToGL( const QMouseEvent& event )
{
    return ScreenToGL( event.pos() );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::ScreenToGL
// Created: APE 2004-02-27
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GLWidget::ScreenToGL( const QPoint& vScreenPos )
{
    return ScreenToGL( MT_Vector2D( vScreenPos.x(), vScreenPos.y() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::ScreenToGL
// Created: AGE 2005-03-16
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GLWidget::ScreenToGL( const MT_Vector2D& v )
{
    const MT_Float rXRatio = v.rX_ / MT_Float( width() );
    const MT_Float rYRatio = v.rY_ / MT_Float( height() );

    return MT_Vector2D( viewport_.GetLeft() + rXRatio * viewport_.GetWidth() 
                      , viewport_.GetTop()  - rYRatio * viewport_.GetHeight() );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::PushMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_GLWidget::PushMapEventFilter( MOS_MapEventFilter_ABC& filter )
{
    if( filter.IsBlocking() )
    {
        for( RIT_MapEventFilterVector it = eventFilterStack_.rbegin(); it != eventFilterStack_.rend(); ++it )
        {
            this->removeEventFilter( (*it)->GetObject() );
            if( (*it)->IsBlocking() )
                break;
        }
    }

    this->installEventFilter( filter.GetObject() );
    eventFilterStack_.push_back( &filter );
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::PopMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_GLWidget::PopMapEventFilter( MOS_MapEventFilter_ABC& filter )
{
    assert( ! eventFilterStack_.empty() );
    assert( eventFilterStack_.back() == &filter );

    this->removeEventFilter( filter.GetObject() );
    eventFilterStack_.pop_back();

    for( RIT_MapEventFilterVector it = eventFilterStack_.rbegin(); it != eventFilterStack_.rend(); ++it )
    {
        this->installEventFilter( (*it)->GetObject() );
        if( (*it)->IsBlocking() )
            break;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::GetMapEvents
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
const T_MapEventFilterVector& MOS_GLWidget::GetMapEvents() const
{
    return eventFilterStack_;
}
