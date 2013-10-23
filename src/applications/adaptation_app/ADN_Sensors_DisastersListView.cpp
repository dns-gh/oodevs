// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_DisastersListView.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"

namespace
{
    // Internal connector
    class ADN_Connector_Disasters : public ADN_Connector_ListView_ABC
    {

    public:
        //! @name Constructors/Destructor
        //@{
        explicit ADN_Connector_Disasters( ADN_ListView& list ) : ADN_Connector_ListView_ABC( list ) {};
        virtual ~ADN_Connector_Disasters() {};
        //@}

        //! @name Operations
        //@{
        ADN_ListViewItem* CreateItem( void* pObj )
        {
            ADN_Sensors_Data::DisasterInfos* infos = static_cast< ADN_Sensors_Data::DisasterInfos* >( pObj );
            if( !infos || !infos->GetCrossedElement() )
                return 0;
            ADN_ListViewItem* pItem = new ADN_ListViewItem( pObj );
            pItem->Connect( &infos->GetCrossedElement()->strName_ );
            return pItem;
        }
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DisastersListView constructor
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
ADN_Sensors_DisastersListView::ADN_Sensors_DisastersListView( QWidget* pParent /* = 0 */ )
    : ADN_ListView( pParent, "ADN_Sensors_DisastersListView", tools::translate( "ADN_Sensors_DisastersListView", "Disasters" ) )
{
    pConnector_.reset( new ADN_Connector_Disasters( *this ) );
    SetDeletionEnabled( true );
    setMaximumHeight( 150 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DisastersListView destructor
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
ADN_Sensors_DisastersListView::~ADN_Sensors_DisastersListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DisastersListView::ConnectItem
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DisastersListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Sensors_Data::DisasterInfos* pInfos = (ADN_Sensors_Data::DisasterInfos*)pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Sensors_GUI::eNbrDisasterElements );

    vItemConnectors_[ADN_Sensors_GUI::eDetectionThreshold]->Connect( &pInfos->rDetectionThreshold_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_DisastersListView::OnContextMenu
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Sensors_DisastersListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( &popupMenu );

    ADN_Disasters_Data::T_DisasterInfos_Vector& vDisasters = ADN_Workspace::GetWorkspace().GetDisasters().GetData().GetDisastersInfos();
    for( auto it = vDisasters.begin(); it != vDisasters.end(); ++it )
    {
        ADN_Disasters_Data::DisasterInfos* pDisaster = *it;
        if( Contains( pDisaster ) )
            continue;
        addMenu.insertItem( pDisaster->strName_.GetData().c_str(), 2 + static_cast< int >( std::distance( vDisasters.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tools::translate( "ADN_Sensors_DisastersListView", "Add disaster" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tools::translate( "ADN_Sensors_DisastersListView", "Remove disaster" ), 1 );

    int nMenuResult = popupMenu.exec( pt );

    if( nMenuResult == 1 )
    {
        ADN_Sensors_Data::DisasterInfos* pCurObject = (ADN_Sensors_Data::DisasterInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurObject );
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        ADN_Sensors_Data::DisasterInfos* pNewInfo = new ADN_Sensors_Data::DisasterInfos();
        pNewInfo->SetCrossedElement( vDisasters[ nMenuResult - 2 ] );

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_DisastersListView::Contains
// Created: LGY 2012-12-03
//-----------------------------------------------------------------------------
bool ADN_Sensors_DisastersListView::Contains( const ADN_Disasters_Data::DisasterInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Sensors_Data::DisasterInfos* pData = static_cast< ADN_Sensors_Data::DisasterInfos* >( pCurr->GetData() );
        if( pData->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}
