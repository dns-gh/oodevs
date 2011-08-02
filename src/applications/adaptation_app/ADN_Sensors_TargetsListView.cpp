// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_TargetsListView.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 16:42 $
// $Revision: 5 $
// $Workfile: ADN_Sensors_TargetsListView.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_TargetsListView.h"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Sensors_Data.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Sensors_Data::TargetInfos TargetInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_TargetsListView constructor
// Created: APE 2005-01-18
// -----------------------------------------------------------------------------
ADN_Sensors_TargetsListView::ADN_Sensors_TargetsListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Targets" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<TargetInfos>(*this);

    this->SetDeletionEnabled( true );
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_TargetsListView destructor
// Created: APE 2005-01-18
// -----------------------------------------------------------------------------
ADN_Sensors_TargetsListView::~ADN_Sensors_TargetsListView()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_TargetsListView::ConnectItem
// Created: APE 2005-01-18
// -----------------------------------------------------------------------------
void ADN_Sensors_TargetsListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    TargetInfos* pInfos = (TargetInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Sensors_GUI::eNbrObjGuiElements );

    vItemConnectors_[ADN_Sensors_GUI::eObjDistDetect]->Connect( &pInfos->rDistanceDetection_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eObjModifStances]->Connect( &pInfos->vModifStance_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eObjPopulationDensity]->Connect( &pInfos->populationInfos_.rDensity_, bConnect );
    vItemConnectors_[ADN_Sensors_GUI::eObjPopulationModifier]->Connect( &pInfos->populationInfos_.rModifier_, bConnect );
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_TargetsListView::OnContextMenu
// Created: APE 2005-01-18
// -----------------------------------------------------------------------------
void ADN_Sensors_TargetsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( &popupMenu );

    // Add the available targets to the 'add target' submenu.
    ADN_Objects_Data::T_ObjectsInfos_Vector& vObjects = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();
    for( ADN_Objects_Data::IT_ObjectsInfos_Vector it = vObjects.begin(); it != vObjects.end(); ++it )
    {
        ADN_Objects_Data::ObjectInfos* pObject = *it;
        // Don't add a object to the menu if it already is present in the list.
        if( Contains( pObject ) )
            continue;
        addMenu.insertItem( pObject->strName_.GetData().c_str(), (int)pObject );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add object" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove object" ), 1 );

    int nMenuResult = popupMenu.exec( pt );

    if( nMenuResult == 1 )
    {
        // Remove the weapon from the list.
        TargetInfos* pCurObject = (TargetInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurObject );
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        TargetInfos* pNewInfo = new TargetInfos();
        pNewInfo->ptrObject_ = (ADN_Objects_Data::ObjectInfos*)nMenuResult;
        pNewInfo->strName_ = pNewInfo->ptrObject_.GetData()->strName_.GetData();

        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        setCurrentItem( FindItem( pNewInfo ) );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_TargetsListView::Contains
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
bool ADN_Sensors_TargetsListView::Contains( const ADN_Objects_Data::ObjectInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        TargetInfos* pData = static_cast< TargetInfos* >( pCurr->GetData() );
        if( pData->ptrObject_.GetData() == pInfo )
            return true;
        ++it;
    }
    return false;
}
