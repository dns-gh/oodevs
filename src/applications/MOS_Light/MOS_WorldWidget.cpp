//*****************************************************************************
//
// $Created: NLD 2002-11-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_WorldWidget.cpp $
// $Author: Age $
// $Modtime: 7/02/05 17:35 $
// $Revision: 8 $
// $Workfile: MOS_WorldWidget.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_WorldWidget.h"

#ifndef MOS_USE_INLINE
#   include "MOS_WorldWidget.inl"
#endif

#include "MOS_GLWidget.h"
#include "MOS_Scroller.h"
#include "MOS_ZoomSlider.h"

#include <qslider.h>

//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget constructor
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_WorldWidget::MOS_WorldWidget( QWidget* pParent )
    : QFrame        ( pParent )
{
    pGLWidget_      = new MOS_GLWidget( this );
    
    pScroller_      = new MOS_Scroller( this, pGLWidget_ );
    pScroller_->hide();

    pZoomSlider_    = new MOS_ZoomSlider( this );
    
    QHBoxLayout* pMainLayout = new QHBoxLayout( this );
    pMainLayout->addWidget( pGLWidget_ );

    QVBoxLayout* pLayout = new QVBoxLayout( pMainLayout );
    pLayout->addWidget( pScroller_ );
    pLayout->addWidget( pZoomSlider_ );

    connect( pZoomSlider_, SIGNAL( valueChanged(int) ), pGLWidget_, SLOT( SlotSetZoom(int)) );

    pGLWidget_->setFocusProxy( pZoomSlider_ );

    setMinimumWidth( 800 );
    setMinimumHeight( 400 );

}


//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget destructor
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_WorldWidget::~MOS_WorldWidget()
{
    delete pGLWidget_;
    delete pZoomSlider_;
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget::Initialize
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_WorldWidget::Initialize()
{
    pGLWidget_->Initialize();

    // $$$$ AGE 2005-02-07: 
    pZoomSlider_->setMinValue( 1 );
    pZoomSlider_->setMaxValue( 100 );
    pZoomSlider_->setPageStep( 1 );
    pZoomSlider_->setValue   ( 100 );

    setMinimumWidth( 500 );

}

//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget::Terminate
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_WorldWidget::Terminate()
{
    pGLWidget_->Terminate();
}


// -----------------------------------------------------------------------------
// Name: MOS_WorldWidget::wheelEvent
/** @param  pEvent 
*/
// Created: AGN 2004-04-20
// -----------------------------------------------------------------------------
void MOS_WorldWidget::wheelEvent( QWheelEvent* pEvent )
{
    if( pZoomSlider_ != 0 )
        pZoomSlider_->HandleExternalWheelEvent( pEvent );
}
