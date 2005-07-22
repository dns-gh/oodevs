// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GL3DWidget.cpp $
// $Author: Age $
// $Modtime: 17/06/05 10:17 $
// $Revision: 6 $
// $Workfile: MOS_GL3DWidget.cpp $
//
// *****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_GL3DWidget.h"
#include "MT_GLDrawer.h"
#include "MOS_MainWindow.h"
#include "MOS_GLTool.h"
#include "MT_GLFont.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_MapMouseEvent.h"
#include "MOS_MapDropEvent.h"
#include "moc_MOS_GL3DWidget.cpp"

namespace
{
    // $$$$ AGE 2005-03-16: ...
    // Helper function used for the MT_GLDrawer
    MT_Vector2D ScreenToGL( const MT_Vector2D& screenPos, QGLWidget& glWidget )
    {
        return ((MOS_GL3DWidget&)glWidget).ScreenToGL( QPoint( screenPos.rX_, screenPos.rY_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget constructor
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
MOS_GL3DWidget::MOS_GL3DWidget( QWidget* pParent )
    : MapWidget3D( pParent, MOS_App::GetApp().GetWorld().GetTerrainDirectory() ) // $$$$ AGE 2005-05-13: 
    , pGLDrawer_( 0 )
    , rZFactor_( 10 )
    , rMetersPerPixel_( 100 )  // $$$$ AGE 2005-05-19: 
    , viewport_( -std::numeric_limits< MT_Float >::infinity(), -std::numeric_limits< MT_Float >::infinity(), 
                  std::numeric_limits< MT_Float >::infinity(),  std::numeric_limits< MT_Float >::infinity() )
{
    SetZRatio( rZFactor_ );

    glInit();
    pGLDrawer_ = new MT_GLDrawer( *this, &(::ScreenToGL), MOS_MainWindow::GetMainWindow() ); // $$$$ AGE 2005-03-16: ??
    update();

    pTimer_ = new QTimer( this );
    pTimer_->start( 50 );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget destructor
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
MOS_GL3DWidget::~MOS_GL3DWidget()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::event
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
bool MOS_GL3DWidget::event( QEvent* pEvent )
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

//    if( bDealt )
//        Update();
    return bDealt;
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::hideEvent
// Created: AGE 2005-05-16
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::hideEvent( QHideEvent* )
{
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), this,   SLOT( OnCenterOnPoint( const MT_Vector2D& ) ) );
    disconnect( pTimer_, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::showEvent
// Created: AGE 2005-05-16
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::showEvent( QShowEvent* )
{
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ), this,   SLOT( OnCenterOnPoint( const MT_Vector2D& ) ) );
    connect( pTimer_, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::keyPressEvent
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::keyPressEvent( QKeyEvent* pEvent )
{
    if( pEvent->key() == Qt::Key_Plus )
    {
        rZFactor_ *= 1.1f;
        SetZRatio( rZFactor_ );
    }
    else if( pEvent->key() == Qt::Key_Minus )
    {
        rZFactor_ *= 0.9f;
        SetZRatio( rZFactor_ );
    }
    Widget3D::keyPressEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::initializeGL
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::initializeGL()
{
    Widget3D::initializeGL();
//    QFont scipioFont( "Scipio", 64 );
//    renderText( 0, 0, "", scipioFont );
//    MT_GLFont::Font( "Arial", 11.0, ANSI_CHARSET, true, FW_BOLD );
    GFX_Tools::InitializeFonts();
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::paintGL
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::paintGL()
{
    Widget3D::paintGL();
    glBindTexture( GL_TEXTURE_2D, 0 );

    MOS_GLTool::SetAltitudeRatio( rZFactor_ );
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
        MOS_GLTool::DrawLine( vRulerStartPos_, vRulerEndPos_, rMetersPerPixel_ * 25, true ); // $$$$ AGE 2005-03-17: 
        MT_GLFont::Print( "Arial", vPosText, MT_FormatString( "  %5.2fm", rMeters  ), rMetersPerPixel_ * 25 );
    }

	glFlush();
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::mouseMoveEvent
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::mouseMoveEvent( QMouseEvent* pMouseEvent )
{
    Widget3D::mouseMoveEvent( pMouseEvent );
    emit MouseMove( pMouseEvent, ScreenToGL( *pMouseEvent ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::ScreenToGL
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GL3DWidget::ScreenToGL( const MT_Vector2D& v )
{
    return ScreenToGL( QPoint( v.rX_, v.rY_ ) );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::ScreenToGL
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GL3DWidget::ScreenToGL( const QMouseEvent& event )
{
    return ScreenToGL( event.pos() );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::ScreenToGL
// Created: AGE 2005-05-09
// -----------------------------------------------------------------------------
MT_Vector2D MOS_GL3DWidget::ScreenToGL( const QPoint& vScreenPos )
{
    geometry::Point3f p = PointAt( vScreenPos );
    return MT_Vector2D( p.X(), p.Y() );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::OnCenterOnPoint
// Created: AGE 2005-05-13
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::OnCenterOnPoint( const MT_Vector2D& vPoint )
{
    CenterOn( geometry::Point2f( vPoint.rX_, vPoint.rY_ ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_GL3DWidget::PushMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::PushMapEventFilter( MOS_MapEventFilter_ABC& filter )
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
// Name: MOS_GL3DWidget::PopMapEventFilter
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
void MOS_GL3DWidget::PopMapEventFilter( MOS_MapEventFilter_ABC& filter )
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
// Name: MOS_GL3DWidget::GetMapEvents
// Created: AGE 2005-05-17
// -----------------------------------------------------------------------------
const T_MapEventFilterVector& MOS_GL3DWidget::GetMapEvents() const
{
    return eventFilterStack_;
}
