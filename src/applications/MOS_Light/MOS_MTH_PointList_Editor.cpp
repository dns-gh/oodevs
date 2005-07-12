//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_Editor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_MTH_PointList_Editor.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_PointList_Editor.h"
#include "moc_MOS_MTH_PointList_Editor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_PointList_Editor.inl"
#endif

#include "MOS_MTH_PointList_ListViewItem.h"
#include "MOS_MTH_Localisation_ListView.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"

#include "MOS_PointListViewItem.h"


#include <qgrid.h>


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_Editor constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_Editor::MOS_MTH_PointList_Editor( QWidget* pParent )
: QWidget( pParent )
, pLocalisation_( 0 )
, pEditedItem_( 0 )
{
    // create frame - top widget
//    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
//    this->setLineWidth( 1 );
    
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    QLabel* pTitle = new QLabel( "Points", this );
    pTitle->setAlignment( AlignHCenter | AlignVCenter );
    pTitle->setLineWidth( 2 );
    pTitle->setFrameStyle( QFrame::Sunken | QFrame::Box );

    pLocalisation_ = new MOS_MTH_Localisation_ListView( this, "Itinéraire" );
    pLocalisation_->Initialize();

    ///////////////
    // Buttons
    QGrid* pGrid =new QGrid( 2,this );
    QPushButton* pButton = new QPushButton( "Ok", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotOk() ) );
    pButton = new QPushButton( "Cancel", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );    
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_Editor destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_Editor::~MOS_MTH_PointList_Editor()
{
    pLocalisation_->Terminate();
    delete pLocalisation_;
    pLocalisation_ = 0;    
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_Editor::SetEditedItem
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_Editor::SetEditedItem( MOS_MTH_PointList_ListViewItem* pEditedItem )
{
    pEditedItem_ = pEditedItem;
    if( pEditedItem_ != 0 )
    {
        pLocalisation_->DeleteAllPoints();

        const T_PointVector& vPoints = pEditedItem->GetListPoint();

        for( CIT_PointVector it = vPoints.begin(); it != vPoints.end(); ++it )
            pLocalisation_->CreatePoint( *it );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_Editor::SlotOk
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_Editor::SlotOk()
{
    pEditedItem_->ClearList();

    MOS_PointListViewItem* pFirst = pLocalisation_->GetFirstPoint();
    while( pFirst != 0 )
    {
        pEditedItem_->AddPoint( pFirst->GetPoint() );
        pFirst = pLocalisation_->GetPointAfter( *pFirst );
    }
    
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_Editor::SlotCancel
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_Editor::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}

