//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GLWidget.cpp $
// $Author: Nld $
// $Modtime: 4/04/05 17:30 $
// $Revision: 37 $
// $Workfile: MOS_GLWidget.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_GLWidget.h"
#include "moc_MOS_GLWidget.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_GLWidget.inl"
#endif

#include "MOS_World.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_LineManager.h"
#include "MOS_MainWindow.h"
#include "MOS_LimaEditor.h"
#include "MOS_LimaSelector.h"
#include "MOS_LimaListView.h"
#include "MOS_AgentListView.h"
#include "MOS_WorldWidget.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_AgentEditor.h"
#include "MOS_LimitListView.h"
#include "MOS_LimaListView.h"
#include <qtimer.h>
#include <qslider.h>

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_GLWidget::MOS_GLWidget( QWidget* pParent )
    : QGLWidget         ( QGLFormat( StencilBuffer ), pParent )
    , nDrawMeasure_     ( 0 )
    , vPosStartMeasure_ ( 0.0, 0.0 )
    , vPosEndMeasure_   ( 0.0, 0.0 )
    , bGrab_            ( false )
    , rZoom_( 0.5 )
    , rWantedZoom_( 0.5 )
    , rMaxZoom_( 0.5 )
{
    glInit();

    pTimer_ = new QTimer( this );
    connect( pTimer_, SIGNAL(timeout()), SLOT(update()) );
    
    setMouseTracking( true );
    setFocusPolicy( QWidget::ClickFocus );
    Update();
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget destructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_GLWidget::~MOS_GLWidget()
{
    GFX_Tools::DeleteFonts();
    pTimer_->stop();
    delete pTimer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_GLWidget::Initialize()
{
    SetCenter      ( MT_Vector2D( 0, 0 ) );
    SetWantedCenter( MT_Vector2D( 0, 0 ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_GLWidget::Terminate()
{
}

//=============================================================================
// QT
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::initializeGL
// Created: FBD 02-06-11
//-----------------------------------------------------------------------------
void MOS_GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable   ( GL_BLEND );
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    GFX_Tools::InitializeFonts();
    renderText( 0, 0, "" );
    QFont font;
    font.setPointSize( 16 );
    renderText( 0, 0, "", font );
}
    
//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::resizeGL
// Created: FBD 02-06-11
//-----------------------------------------------------------------------------
void MOS_GLWidget::resizeGL( int w, int h )
{
    SetCurrentGL();
    CalculateViewport( w, h );
    glViewport(0, 0, w, h);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( viewport_.GetLeft(), viewport_.GetRight()
           , viewport_.GetBottom(), viewport_.GetTop()
           , -300, 300 );
    Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::CalculateViewport
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::CalculateViewport( int w, int h  )
{
    rRatio_ = float( w ) / float( h );
    MOS_World* pWorld = & MOS_App::GetApp().GetWorld(); // $$$$ AGE 2005-03-11: Singletons are evil
    const double rHalfHeight = pWorld ? 0.5 * pWorld->GetRect().GetWidth() / rZoom_ : 0;
    const double rHalfWidth  =  rHalfHeight * rRatio_;
    viewport_.Set( MT_Vector2D( vCenter_.rX_ - rHalfWidth, vCenter_.rY_ - rHalfHeight )
                 , MT_Vector2D( vCenter_.rX_ + rHalfWidth, vCenter_.rY_ + rHalfHeight ) );
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::paintGL
// Created: FBD 02-06-11
//-----------------------------------------------------------------------------
void MOS_GLWidget::paintGL()
{
    bUpdating_ = true;
    SetCurrentGL();
    MOS_World& world = MOS_App::GetApp().GetWorld();

    UpdateCenter();
    resizeGL( width(), height() );

    glClear( GL_COLOR_BUFFER_BIT );
    glClear( GL_DEPTH_BUFFER_BIT );

     // $$$$ AGE 2005-02-07: 
//    const bool bOptimizeDraw = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsOptiDraw();

    world.Draw( viewport_ );

    GFX_Tools::ClipBordersRect( world.GetRect() ); // $$$$ AGE 2005-02-07: ??

    MOS_App::GetApp().GetAgentManager().DrawVisionSurface();
    MOS_App::GetApp().GetAgentManager().DrawVisionCone();
    MOS_App::GetApp().GetAgentManager().Draw();
    MOS_App::GetApp().GetAgentManager().DrawVision();

    MOS_App::GetApp().GetDynaObjectManager().Draw();

    DrawLink();

    MOS_App::GetApp().GetMainWindow().GetAgentEditor().Draw();

    if( MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsDrawAllLima() )
    {
        MOS_App::GetApp().GetMainWindow().GetLimaEditor().Draw();
    }

    if( MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsDrawAllLimit() )
    {
        MOS_App::GetApp().GetMainWindow().GetLimitEditor().Draw();
    }

    bool bDrawText = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsDrawText();
    if( bDrawText && viewport_.GetWidth() < 15000 )
        world.DrawNameObjects( viewport_, *this );

    DrawMeasure();

    for( IT_ConflictMap it = conflicts_.begin(); it != conflicts_.end(); ++it )
    {
        GFX_Tools::CreateGLArrow( (*it).second.first, (*it).second.second, GFX_Color(255, 0, 0), 5, 100 );
    }

    MOS_App::GetApp().GetRawVisionData().DrawAmmoMeteoEffects();

    glPopMatrix();
    glFlush();
    SetupNextUpdate();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SetupNextUpdate
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SetupNextUpdate()
{
    bUpdating_ = false;
    if( fabs( rWantedZoom_ - rZoom_ ) > 0.02 
     || vWantedCenter_.Distance( vCenter_ ) * rZoom_ > 500 )
        Update( 40 );
    else
        Update( 80 );
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::SetWantedCenter
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_GLWidget::SetWantedCenter( const MT_Vector2D& vWantedCenter )
{
    vWantedCenter_ = vWantedCenter;
    Update();
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::UpdateCenter
// Created: FBD 02-06-14
//-----------------------------------------------------------------------------
void MOS_GLWidget::UpdateCenter()
{
    vCenter_  += ( vWantedCenter_ - vCenter_ ) * 0.5;
    rZoom_    += ( rWantedZoom_ - rZoom_ ) * 0.5;
}


//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::ScreenToGL
// Created: FBD 02-08-22
//-----------------------------------------------------------------------------
void MOS_GLWidget::ScreenToGL( const MT_Vector2D& vPosScreen, MT_Vector2D& vPosGL )
{
    const MT_Float rXRatio = vPosScreen.rX_ / width();
    const MT_Float rYRatio = vPosScreen.rY_ / height();
    vPosGL.rX_ = viewport_.GetLeft() + rXRatio * viewport_.GetWidth();
    vPosGL.rY_ = viewport_.GetTop()  - rYRatio * viewport_.GetHeight();
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetCenter
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetCenter()
{
    MOS_World& world = MOS_App::GetApp().GetWorld();
    vWantedCenter_ = world.GetRect().GetCenter();
    Update();
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::RegisterLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::RegisterLink( MOS_GLMouse_ABC* pLink )
{
    IT_GLMousePtrVector itLink = std::find( linkVector_.begin() , linkVector_.end() , pLink );
    assert( itLink == linkVector_.end() );
    linkVector_.push_back( pLink );
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::UnregisterLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::UnregisterLink( MOS_GLMouse_ABC* pLink )
{
    IT_GLMousePtrVector itLink = std::find( linkVector_.begin(), linkVector_.end(), pLink );
    assert( itLink != linkVector_.end() );
    linkVector_.erase( itLink );
}
//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::mousePressEvent
// Created: FBD 02-06-13
//-----------------------------------------------------------------------------
void MOS_GLWidget::mousePressEvent( QMouseEvent* pMouseEvent )
{
    MT_Vector2D vPosMouse( pMouseEvent->x(), pMouseEvent->y() );
    MT_Vector2D vPosGL;
    ScreenToGL( vPosMouse, vPosGL );

    if( pMouseEvent->button() == MidButton
      || ( pMouseEvent->button() == LeftButton && pMouseEvent->state() == ControlButton ) )
    {
        vGrabCoordinates_ = vPosMouse;
    }
    else
    if( pMouseEvent->button() == LeftButton )
    {
        //SetWantedCenter( vPosGL );
        LMBPressEventLink( vPosGL );
    }
    else
    if( pMouseEvent->button() == RightButton )
    {
        RMBPressEventLink( vPosGL );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::mouseMoveEvent
// Created: NLD 2002-11-29
//-----------------------------------------------------------------------------
void MOS_GLWidget::mouseMoveEvent( QMouseEvent* pMouseEvent )
{
    MT_Vector2D vPosMouse( pMouseEvent->x(), pMouseEvent->y() );
    MT_Vector2D vPosGL;
    ScreenToGL( vPosMouse, vPosGL );

    if( pMouseEvent->state() == MidButton
      || ( pMouseEvent->state() == (LeftButton | ControlButton) ))
    {
        // Compute the screen displacement (invert Y coords).
        MT_Vector2D vDisplacement = vGrabCoordinates_ - vPosMouse;

        // Compute the GL displacement length.
        MT_Vector2D vGLDisplacement;
        MT_Vector2D vGLZero;
        ScreenToGL( MT_Vector2D(0.0,0.0), vGLZero );
        ScreenToGL( vDisplacement, vGLDisplacement );
        vGLDisplacement = vGLDisplacement - vGLZero;

        // Update both vWantedCenter_ and vCenter_ to prevent mouvement smoothing.
        vWantedCenter_ = vCenter_ + vGLDisplacement;
        vCenter_ = vCenter_ + vGLDisplacement;
        vGrabCoordinates_ = vPosMouse;
        Update();
    }
    else
        mouseMoveEventLink( vPosGL );

    if( MOS_App::GetApp().GetWorld().GetRect().IsInside( vPosGL ) )
    {
        std::string strMousePos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPosGL, strMousePos );
        MOS_App::GetApp().GetMainWindow().NotifyMousePositionHasChanged( strMousePos, vPosGL );
    }
    else
    {
        MOS_App::GetApp().GetMainWindow().NotifyMousePositionHasChanged( "Outside world", vPosGL );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::mouseReleaseEvent
// Created: FBD 02-12-03
//-----------------------------------------------------------------------------
void MOS_GLWidget::mouseReleaseEvent( QMouseEvent* pMouseEvent )
{
    MT_Vector2D vPosMouse( pMouseEvent->x(), pMouseEvent->y() );
    MT_Vector2D vPosGL;
    ScreenToGL( vPosMouse, vPosGL );

    if( pMouseEvent->button() == LeftButton )
    {
        LMBReleaseEventLink( vPosGL );
    }
    else
    if( pMouseEvent->button() == RightButton )
    {
        RMBReleaseEventLink( vPosGL );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::DrawLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::DrawLink()
{
    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    if( (*itLink)->IsActive( false ) )
        (*itLink)->Draw();
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::RMBPressEventLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_GLWidget::RMBPressEventLink( const MT_Vector2D& vPos )
{
    bool bReturn = false;
    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    {
        if( (*itLink)->IsActive( true ) )
        {
            MOS_GLMouse_ABC* pMouse = (*itLink);
            bReturn |= pMouse->RMBPressEvent( vPos );
            if( bReturn )
                break;
        }
    }

    if( !bReturn && nDrawMeasure_ == 0 )
    {
        nDrawMeasure_ = 1;
        vPosStartMeasure_ = vPos;
        vPosEndMeasure_ = vPos;
    }
    return bReturn;
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::LMBPressEventLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_GLWidget::LMBPressEventLink( const MT_Vector2D& vPos )
{
    bool bReturn = false;

    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    {
        if( (*itLink)->IsActive( true ) )
            bReturn |= (*itLink)->LMBPressEvent( vPos );
    }
    if( nDrawMeasure_ )
        nDrawMeasure_ = 0;

    return bReturn;

}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::mouseMoveEventLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::mouseMoveEventLink( const MT_Vector2D& vPos )
{
    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    {
        if( (*itLink)->IsActive( true ) )
        {
            (*itLink)->MouseMoveEvent( vPos );
        }
    }

    if( nDrawMeasure_ == 1 )
    {
        vPosEndMeasure_ = vPos;
        MT_Float rDistance = vPosStartMeasure_.Distance( vPosEndMeasure_ );

        if( rDistance > 100. )
            nDrawMeasure_ = 2;

    }
    else
    if( nDrawMeasure_ == 2 )
       vPosEndMeasure_ = vPos;
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetZoom
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetZoom( int i )
{
    // 1 <= i <= 100
    if( i > 90 )
        rWantedZoom_ = 10.5 - 0.1 * i; // 0.5 -> 1.5
    else if( i > 80 )
        rWantedZoom_ = 19.5 - 0.2 * i; // 1.5 -> 3.5
    else if( i > 50 )
        rWantedZoom_ = 35.5 - 0.4 * i; // 3.5 -> 15.5
    else
        rWantedZoom_ = 55.5 - 0.8 * i; // 15.5 -> 55.5
    Update();
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::RMBReleaseEventLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::RMBReleaseEventLink( const MT_Vector2D& vPos )
{
    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    if( (*itLink)->IsActive( true ) )
        (*itLink)->RMBReleaseEvent( vPos );

    if( nDrawMeasure_ )
        nDrawMeasure_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::LMBReleaseEventLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLWidget::LMBReleaseEventLink( const MT_Vector2D& vPos )
{
    for( IT_GLMousePtrVector itLink = linkVector_.begin() ; itLink != linkVector_.end() ; ++itLink )
    if( (*itLink)->IsActive( true ) )
        (*itLink)->LMBReleaseEvent( vPos );

    if( nDrawMeasure_ )
        nDrawMeasure_ = 0;

}
//-----------------------------------------------------------------------------
//  Name  :  MOS_GLWidget::DrawMeasure
// Created: FBD 02-08-27
//-----------------------------------------------------------------------------
void MOS_GLWidget::DrawMeasure()
{
     GFX_Color color( 255,255,255., 0.9 );
     MT_Float rSizearrow = 100.;

    if( nDrawMeasure_ == 2 )
    {
        if( vPosStartMeasure_ == vPosEndMeasure_ )
            return;

        GFX_Tools::CreateGLArrow( vPosStartMeasure_, vPosEndMeasure_, color, 2.0, rSizearrow );
        GFX_Tools::CreateGLArrow( vPosEndMeasure_, vPosStartMeasure_, color, 2.0, rSizearrow );

        //MOS_World& world = MOS_App::GetApp().GetWorld();
        MT_Float rMeters = vPosStartMeasure_.Distance( vPosEndMeasure_ );

        char cMeters[256];
        sprintf( cMeters, "%5.2f Meters", rMeters );
        MT_Vector2D vPosText =  MT_Vector2D( ( vPosEndMeasure_ - vPosStartMeasure_ ) / 2. ) + vPosStartMeasure_ + MT_Vector2D( 800., 0. );
        color.SetGLColor();

        QFont font;
        font.setPointSize( 16 );
        renderText( vPosText.rX_, vPosText.rY_, 0, cMeters, font );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetXTranslationLeft
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetXTranslationLeft()
{
    vWantedCenter_.rX_ = vCenter_.rX_ - viewport_.GetWidth() * 0.75;
    Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetXTranslationRight
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetXTranslationRight()
{
    vWantedCenter_.rX_ = vCenter_.rX_ + viewport_.GetWidth() * 0.75;
    Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetYTranslationUp
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetYTranslationUp()
{
    vWantedCenter_.rY_ = vCenter_.rY_ + viewport_.GetHeight() * 0.75;
    Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::SlotSetYTranslationDown
// Created: AGE 2005-02-07
// -----------------------------------------------------------------------------
void MOS_GLWidget::SlotSetYTranslationDown()
{
    vWantedCenter_.rY_ = vCenter_.rY_ + viewport_.GetHeight() * 0.75;
    Update();
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
// Name: MOS_GLWidget::AddConflict
/** @param  nId
    @param  vStart
    @param  vEnd
*/
// Created: APE 2004-03-12
// -----------------------------------------------------------------------------
void MOS_GLWidget::AddConflict( uint nId, const MT_Vector2D& vStart, const MT_Vector2D& vEnd )
{
    conflicts_[ nId ] = T_VectorPair( vStart, vEnd );
}


// -----------------------------------------------------------------------------
// Name: MOS_GLWidget::DeleteConflict
/** @param  nId
*/
// Created: APE 2004-03-12
// -----------------------------------------------------------------------------
void MOS_GLWidget::DeleteConflict( uint nId )
{
    IT_ConflictMap it = conflicts_.find( nId );
    if( it != conflicts_.end() )
        conflicts_.erase( it );
}
