// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Equipments_Objects.h"
#include "moc_ADN_ListView_Equipments_Objects.cpp"
#include <Qt3Support/q3popupmenu.h>
#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Equipments_GUI.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Equipments_Data::ObjectInfos ObjectInfos;

// Internal connector
class ADN_Connector_Objects : public ADN_Connector_ListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Connector_Objects( ADN_ListView& list ) : ADN_Connector_ListView_ABC( list ) {};
    virtual ~ADN_Connector_Objects() {};
    //@}

    //! @name Operations
    //@{
    ADN_ListViewItem* CreateItem( void* pObj )
    {
        // Create the new list item.
        ADN_ListViewItem* pItem = new ADN_ListViewItem( pObj );

        // Connect list item with object's name
        ObjectInfos* infos = static_cast< ObjectInfos* >( pObj );
        if( infos->GetCrossedElement() )
            pItem->Connect( &infos->GetCrossedElement()->strName_ );
        return pItem;
    }
    //@}
};

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Equipments_Objects::ADN_ListView_Equipments_Objects( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_Equipments_Objects", tr( "Objects" ) )
{
    pConnector_.reset( new ADN_Connector_Objects( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments_Objects destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Equipments_Objects::~ADN_ListView_Equipments_Objects()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments_Objects::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Equipments_Objects::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ObjectInfos* pInfos = (ObjectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Equipments_GUI::eNbrObjGuiElements );

    vItemConnectors_[ADN_Equipments_GUI::eInitialBuildTime]->Connect( &pInfos->initialBuildTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eInitialDestructionTime]->Connect( &pInfos->initialDestructionTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCoeffBuildTime]->Connect( &pInfos->coeffBuildTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCoeffDestructionTime]->Connect( &pInfos->coeffDestructionTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eValorizationTime]->Connect( &pInfos->valorizationTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eDevalorizationTime]->Connect( &pInfos->devalorizationTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eExtinguishingTime]->Connect( &pInfos->extinguishingTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eCoeffCircTime]->Connect( &pInfos->rCoeffCirc_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSpeedCirc]->Connect( &pInfos->rSpeedCirc_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eSpeedNotCirc]->Connect( &pInfos->rSpeedNotCirc_, bConnect );

    vItemConnectors_[ADN_Equipments_GUI::eHasInitialBuildTime]->Connect( &pInfos->bInitialBuildTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasInitialDestructionTime]->Connect( &pInfos->bInitialDestructionTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasCoeffBuildTime]->Connect( &pInfos->bCoeffBuildTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasCoeffDestructionTime]->Connect( &pInfos->bCoeffDestructionTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasValorizationTime]->Connect( &pInfos->bValorizationTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasDevalorizationTime]->Connect( &pInfos->bDevalorizationTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasExtinguishingTime]->Connect( &pInfos->bExtinguishingTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasCoeffCircTime]->Connect( &pInfos->bCoeffCircTime_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasSpeedCirc]->Connect( &pInfos->bSpeedCirc_, bConnect );
    vItemConnectors_[ADN_Equipments_GUI::eHasSpeedNotCirc]->Connect( &pInfos->bSpeedNotCirc_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments_Objects::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Equipments_Objects::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available objects to the 'add objects' submenu.
    ADN_Objects_Data::T_ObjectsInfos_Vector& vObjects = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();
    for( auto it = vObjects.begin(); it != vObjects.end(); ++it )
    {
        ADN_Objects_Data_ObjectInfos* pObject = *it;
        // Don't add a object to the menu if it already is present in the list.
        if( Contains( pObject ) )
            continue;
        addMenu.insertItem( pObject->strName_.GetData().c_str(), static_cast< int>( 2 + std::distance( vObjects.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add object" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove object" ), 1 );

    int nMenuResult = popupMenu.exec( pt );

    if( nMenuResult == 1 )
    {
        // Remove the weapon from the list.
        ObjectInfos* pCurObject = (ObjectInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurObject );
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        ObjectInfos* pNewInfo = new ObjectInfos();
        pNewInfo->SetCrossedElement( vObjects[ nMenuResult - 2 ] );

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Equipments_Objects::Contains
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
bool ADN_ListView_Equipments_Objects::Contains( const ADN_Objects_Data_ObjectInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Equipments_Data::ObjectInfos* pData = static_cast< ADN_Equipments_Data::ObjectInfos* >( pCurr->GetData() );
        if( pData->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}
