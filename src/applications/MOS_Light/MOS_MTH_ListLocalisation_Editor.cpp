//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_Editor.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_MTH_ListLocalisation_Editor.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_ListLocalisation_Editor.h"
#include "moc_MOS_MTH_ListLocalisation_Editor.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_ListLocalisation_Editor.inl"
#endif

#include "MOS_MTH_ListLocalisation_ListViewItem.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_Localisation_ListView.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"

#include "MOS_PointListViewItem.h"


#include <qgrid.h>

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_Editor constructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_Editor::MOS_MTH_ListLocalisation_Editor( QWidget* pParent )
: QFrame( pParent )
, pLocalisation_( 0 )
, pEditedItem_( 0 )
{
    // create frame - top widget
    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
    this->setLineWidth( 1 );
    
    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    pLocalisation_ = new MOS_MTH_Localisation( this, "Localisation" );
    pLocalisation_->Initialize();
    pLayout->addWidget( pLocalisation_ );

    ///////////////
    // Buttons
    QGrid* pGrid =new QGrid( 2,this );
    QPushButton* pButton = new QPushButton( "Ok", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotOk() ) );
    pButton = new QPushButton( "Cancel", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );    
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_Editor destructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_Editor::~MOS_MTH_ListLocalisation_Editor()
{
    pLocalisation_->Terminate();
    delete pLocalisation_;
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_Editor::SetEditedItem
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_Editor::SetEditedItem( MOS_MTH_ListLocalisation_ListViewItem* pEditedItem )
{
    pEditedItem_ = pEditedItem;
    if( pEditedItem_ != 0 )
    {
        pLocalisation_->GetListView().DeleteAllPoints();

        pLocalisation_->SetLocalisationType( pEditedItem->GetType() );

        const T_PointVector& vPoints = pEditedItem->GetLocalisation();

        for( CIT_PointVector it = vPoints.begin(); it != vPoints.end(); ++it )
            pLocalisation_->GetListView().CreatePoint( *it );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_Editor::SlotOk
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_Editor::SlotOk()
{
    pEditedItem_->SetType( pLocalisation_->GetLocalisationType() );

    pEditedItem_->ClearList();

    MOS_PointListViewItem* pFirst = pLocalisation_->GetListView().GetFirstPoint();
    while( pFirst != 0 )
    {
        pEditedItem_->AddPoint( pFirst->GetPoint() );
        pFirst = pLocalisation_->GetListView().GetPointAfter( *pFirst );
    }
    
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_Editor::SlotCancel
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_Editor::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}

