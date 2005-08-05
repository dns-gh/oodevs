//*****************************************************************************
// 
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_Editor.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 16:54 $
// $Revision: 19 $
// $Workfile: MOS_MTH_Obstacle_Editor.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_Obstacle_Editor.h"
#include "moc_MOS_MTH_Obstacle_Editor.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_Obstacle_Editor.inl"
#endif

#include "MOS_MTH_Obstacle_ListViewItem.h"
#include "MOS_MTH_Localisation.h"
#include "MOS_MTH_Localisation_ListView.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"

#include "MOS_PointListViewItem.h"


#include <qgrid.h>
#include <qlabel.h>
#include <qcombobox.h>

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_Editor constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_Editor::MOS_MTH_Obstacle_Editor( QWidget* pParent )
: QFrame( pParent )
, pLocalisation_( 0 )
, pEditedItem_( 0 )
, pTypeObstacleGUI_( 0 )
, pUrgenceGUI_( 0 )
, pPreliminaireGUI_( 0 )
, pPrioriteGUI_( 0 )
{
    QBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setMargin( 20 );
    pLayout->setSpacing( 20 );
    pLayout->setAutoAdd( true );

    // Title
    QLabel* pLabel = new QLabel( "Obstacle", this );
    pLabel->setAlignment( AlignHCenter );

    // Combo boxes
    QGrid* pGrid = new QGrid( 2, this );

    pLabel = new QLabel( "Type obstacle", pGrid );
    pTypeObstacleGUI_ = new QComboBox( pGrid );

    for( uint i = 0; i < eNbrObjectType; ++i )
        pTypeObstacleGUI_->insertItem( ENT_Tr::ConvertFromObjectType( (E_ObjectType)i ).c_str(), i );

    pLabel = new QLabel( "Urgence", pGrid );
    pUrgenceGUI_ = new QComboBox( pGrid );
    pUrgenceGUI_->insertItem( "Urgence", EnumMissionGenUrgence::serie_minimum );
    pUrgenceGUI_->insertItem( "Complémentaire", EnumMissionGenUrgence::serie_complementaire );
    pUrgenceGUI_->setCurrentItem( EnumMissionGenUrgence::serie_complementaire );

    pLabel = new QLabel( "Sous type", pGrid );
    pPreliminaireGUI_ = new QComboBox( pGrid );
    pPreliminaireGUI_->insertItem( "Préliminaire", EnumMissionGenSousTypeObstacle::preliminaire );
    pPreliminaireGUI_->insertItem( "De manoeuvre", EnumMissionGenSousTypeObstacle::de_manoeuvre );
    pPreliminaireGUI_->setCurrentItem( EnumMissionGenSousTypeObstacle::de_manoeuvre );

    pLabel = new QLabel( "Priorité", pGrid );
    pPrioriteGUI_ = new QComboBox( pGrid );
    pPrioriteGUI_->insertItem( "Prioritaire", EnumMissionGenPriorite::prioritaire );
    pPrioriteGUI_->insertItem( "Normal", EnumMissionGenPriorite::normal );
    pPrioriteGUI_->insertItem( "Peu prioritaire", EnumMissionGenPriorite::peu_prioritaire );  
    pPrioriteGUI_->setCurrentItem( EnumMissionGenPriorite::normal );

    pLabel = new QLabel( "ID Planifié", pGrid );
    pIDPlanifieGUI_  = new QSpinBox( 0, 100000000, 1, pGrid );

    pLocalisation_ = new MOS_MTH_Localisation( this, "Localisation" );
    pLocalisation_->Initialize();

    ///////////////
    // Buttons
    pGrid =new QGrid( 2,this );
    QPushButton* pButton = new QPushButton( "Ok", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotOk() ) );
    pButton = new QPushButton( "Cancel", pGrid );
    connect( pButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_Editor destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_Editor::~MOS_MTH_Obstacle_Editor()
{
    pLocalisation_->Terminate();
    delete pLocalisation_;
    pLocalisation_ = 0;
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_Editor::SetEditedItem
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_Editor::SetEditedItem( MOS_MTH_Obstacle_ListViewItem* pEditedItem )
{
    pEditedItem_ = pEditedItem;
    if( pEditedItem_ != 0 )
    {
        pLocalisation_->GetListView().DeleteAllPoints();

        pLocalisation_->SetLocalisationType( pEditedItem->GetLocalisationType() );

        const T_PointVector& vPoints = pEditedItem->GetListPoint();

        for( CIT_PointVector it = vPoints.begin(); it != vPoints.end(); ++it )
            pLocalisation_->GetListView().CreatePoint( *it );

        pTypeObstacleGUI_->setCurrentItem( pEditedItem->GetObstacleType() );
        pUrgenceGUI_->setCurrentItem( pEditedItem->GetUrgence() );
        pPreliminaireGUI_->setCurrentItem( pEditedItem->GetSousType() );
        pPrioriteGUI_->setCurrentItem( pEditedItem->GetPriorite() );
        pIDPlanifieGUI_->setValue( pEditedItem->GetIDPlanifie() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_Editor::SlotOk
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_Editor::SlotOk()
{
    pEditedItem_->SetLocalisationType( pLocalisation_->GetLocalisationType() );

    pEditedItem_->ClearList();

    MOS_PointListViewItem* pFirst = pLocalisation_->GetListView().GetFirstPoint();
    while( pFirst != 0 )
    {
        pEditedItem_->AddPoint( pFirst->GetPoint() );
        pFirst = pLocalisation_->GetListView().GetPointAfter( *pFirst );
    }

    pEditedItem_->SetObstacleType( (ASN1T_EnumObjectType)pTypeObstacleGUI_->currentItem() );
    pEditedItem_->SetUrgence( (ASN1T_EnumMissionGenUrgence)pUrgenceGUI_->currentItem() );
    pEditedItem_->SetSousType( (ASN1T_EnumMissionGenSousTypeObstacle)pPreliminaireGUI_->currentItem() );
    pEditedItem_->SetPriorite( (ASN1T_EnumMissionGenPriorite)pPrioriteGUI_->currentItem() );
    pEditedItem_->SetIDPlanifie( pIDPlanifieGUI_->value() );
    
    
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_Editor::SlotCancel
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_Editor::SlotCancel()
{
    MOS_App::GetApp().GetMainWindow().PopWidgetStackLeft();
}

