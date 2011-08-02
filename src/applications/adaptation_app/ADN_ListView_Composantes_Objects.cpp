//*****************************************************************************
//
// $Created: JDY 03-07-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Composantes_Objects.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 9 $
// $Workfile: ADN_ListView_Composantes_Objects.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Composantes_Objects.h"
#include "moc_ADN_ListView_Composantes_Objects.cpp"
#include <Qt3Support/q3popupmenu.h>
#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Composantes_GUI.h"
#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::ObjectInfos ObjectInfos;

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
        ADN_ListViewItem* pItem = new ADN_ListViewItem( &list_, pObj, 1 );

        // Connect list item with object's name
        pItem->Connect( 0, & static_cast<ObjectInfos*>(pObj)->ptrObject_.GetData()->strName_ );
        return pItem;
    }
    //@}
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Composantes_Objects::ADN_ListView_Composantes_Objects( QWidget* pParent, const char* szName, Qt::WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Objects" ) );
//    setResizeMode(QListView::AllColumns);

    setMaximumHeight( 270 );

    // Connector creation.
    pConnector_ = new ADN_Connector_Objects( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes_Objects destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Composantes_Objects::~ADN_ListView_Composantes_Objects()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes_Objects::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Composantes_Objects::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ObjectInfos* pInfos = (ObjectInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Composantes_GUI::eNbrObjGuiElements );

    vItemConnectors_[ADN_Composantes_GUI::eInitialBuildTime]->Connect( &pInfos->initialBuildTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eInitialDestructionTime]->Connect( &pInfos->initialDestructionTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCoeffBuildTime]->Connect( &pInfos->coeffBuildTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCoeffDestructionTime]->Connect( &pInfos->coeffDestructionTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eValorizationTime]->Connect( &pInfos->valorizationTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eDevalorizationTime]->Connect( &pInfos->devalorizationTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eExtinguishingTime]->Connect( &pInfos->extinguishingTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eCoeffCircTime]->Connect( &pInfos->rCoeffCirc_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSpeedCirc]->Connect( &pInfos->rSpeedCirc_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eSpeedNotCirc]->Connect( &pInfos->rSpeedNotCirc_, bConnect );

    vItemConnectors_[ADN_Composantes_GUI::eHasInitialBuildTime]->Connect( &pInfos->bInitialBuildTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasInitialDestructionTime]->Connect( &pInfos->bInitialDestructionTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasCoeffBuildTime]->Connect( &pInfos->bCoeffBuildTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasCoeffDestructionTime]->Connect( &pInfos->bCoeffDestructionTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasValorizationTime]->Connect( &pInfos->bValorizationTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasDevalorizationTime]->Connect( &pInfos->bDevalorizationTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasExtinguishingTime]->Connect( &pInfos->bExtinguishingTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasCoeffCircTime]->Connect( &pInfos->bCoeffCircTime_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasSpeedCirc]->Connect( &pInfos->bSpeedCirc_, bConnect );
    vItemConnectors_[ADN_Composantes_GUI::eHasSpeedNotCirc]->Connect( &pInfos->bSpeedNotCirc_, bConnect );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes_Objects::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_ListView_Composantes_Objects::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available objects to the 'add objects' submenu.
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
        ObjectInfos* pCurObject = (ObjectInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurObject );
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        ObjectInfos* pNewInfo = new ObjectInfos();
        pNewInfo->ptrObject_ = (ADN_Objects_Data::ObjectInfos*)nMenuResult;

        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        setCurrentItem( FindItem( pNewInfo ) );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Composantes_Objects::Contains
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
bool ADN_ListView_Composantes_Objects::Contains( const ADN_Objects_Data::ObjectInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while ( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        ADN_Composantes_Data::ObjectInfos* pData = static_cast< ADN_Composantes_Data::ObjectInfos* >( pCurr->GetData() );
        if( pData->ptrObject_.GetData() == pInfo )
            return true;
        ++it;
    }
    return false;
}
