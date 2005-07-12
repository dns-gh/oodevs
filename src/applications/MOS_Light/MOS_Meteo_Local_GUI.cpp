//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Local_GUI.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Local_GUI.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_Meteo_Local_GUI.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Local_GUI.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Meteo_Local.h"
#include "MOS_Meteo_PointListView.h"
#include "MOS_PointListViewItem.h"

#include <qlayout.h>
#include <qlabel.h>

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Local_GUI::MOS_Meteo_Local_GUI( QWidget* pParent )
: MOS_Meteo_Global_GUI( pParent )
, pTitle_( 0 )
, pPointView_( 0 )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Local_GUI::~MOS_Meteo_Local_GUI()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI::Initialize
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local_GUI::Initialize()
{
    // create frame - top widget
    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
    this->setLineWidth( 1 );
    
    QBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    // Title
    pTitle_ = new QLabel( "Météo Locale", this );
    pTitle_->setAlignment( AlignHCenter | AlignVCenter );
    pTitle_->setLineWidth( 2 );
    pTitle_->setFrameStyle( QFrame::Sunken | QFrame::Box );

    InitializeWidgets();

    new QLabel( "Position de la zone", this );
    pPointView_ = new MOS_Meteo_PointListView( this );
    pPointView_->Initialize();

    InitializeButtons();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI::SlotUpdate
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local_GUI::SlotUpdate()
{
    static_cast< MOS_Meteo_Local* >( pWeather_ )->SetCorners( pPointView_->GetFirstPoint()->GetPoint(), pPointView_->GetPointAfter( *(pPointView_->GetFirstPoint()) )->GetPoint() );
    MOS_Meteo_Global_GUI::SlotUpdate();
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI::SlotCancel
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local_GUI::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Local_GUI::SetWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Local_GUI::SetWeather( MOS_Meteo_ABC* pWeather )
{
    MOS_Meteo_Global_GUI::SetWeather( pWeather );
    if( pWeather_ != 0 )
    {
        MOS_Meteo_Local* pLocal = static_cast< MOS_Meteo_Local* >( pWeather_ );
        std::string strTitle( "Météo locale - Zone " );
        strTitle += itostring( pLocal->GetId() );

        pTitle_->setText( strTitle.c_str() );

        pPointView_->DeleteAllPoints();

        pPointView_->CreatePoint( pLocal->GetUpperLeftCorner() );
        pPointView_->CreatePoint( pLocal->GetLowerRightCorner() );
    }
}

