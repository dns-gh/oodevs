// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Orders.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 7 $
// $Workfile: ADN_ListView_Orders.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Orders.h"

#include "ADN_Connector_ListView.h"
#include "ADN_Models_Data.h"
#include "ADN_StandardItem.h"
#include "ADN_Workspace.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Models_Data::OrderInfos OrderInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::ADN_ListView_Orders( bool usedWithMission, QWidget* parent )
    : ADN_ListView( parent,
                    "ADN_ListView_Orders" + usedWithMission,
                    QString::fromStdString( ENT_Tr::ConvertFromMissionType( eMissionType_FragOrder ) ) )
    , usedWithMission_ ( usedWithMission )
{
    pConnector_.reset( new ADN_Connector_ListView< OrderInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::~ADN_ListView_Orders()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::OnContextMenu
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::OnContextMenu( const QPoint& pt )
{
    fragOrders_.clear();
    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu(this) );

    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData() != 0;

    unsigned int n = 0;
    ADN_Missions_Data::T_Mission_Vector& fragOrders = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder );
    for( auto it = fragOrders.begin(); it != fragOrders.end(); ++it )
    {
        ADN_Missions_FragOrder* fragOrder = static_cast< ADN_Missions_FragOrder* >(*it);
        if( usedWithMission_ || fragOrder->isAvailableWithoutMission_.GetData() )
        {
            std::string strOrderName = fragOrder->strName_.GetData();
            const int id = pTargetMenu->insertItem( strOrderName.c_str(), 2 + n );
            const bool added = Contains( strOrderName );
            pTargetMenu->setItemEnabled( id, !added );
            pTargetMenu->setItemChecked( id, added );
            bDisplayAdd |= !added;
            fragOrders_[ n ] = strOrderName;
            ++n;
        }
    }
    if( ! bDisplayAdd && !bDisplayRem )
        return;

    Q3PopupMenu * pMenu=new Q3PopupMenu(this);
    if( bDisplayAdd )
        pMenu->insertItem( tools::translate( "ADN_ListView_Orders", "Add frag order"), pTargetMenu.get() ,0 );
    if( bDisplayRem )
        pMenu->insertItem( tools::translate( "ADN_ListView_Orders", "Remove frag order"), 1 );

    int nMenu = pMenu->exec( pt );
    if( nMenu == 1 )
        RemoveCurrentItem();
    else if( nMenu > 1 )
        CreateNewItem( nMenu - 2 );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::Contains
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
bool ADN_ListView_Orders::Contains( const std::string& strComposanteName ) const
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem* >( dataModel_.item( row ) );
        if( strComposanteName == pItem->text().toStdString() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::ConnectItem
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::ConnectItem( bool /*bConnect*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::CreateNewItem
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::CreateNewItem( int n )
{
    auto cit = fragOrders_.find( n );
    if( cit != fragOrders_.end() )
    {
        OrderInfos* pNewInfo = new OrderInfos();
        ADN_Missions_Data::T_Mission_Vector& fragOrders = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder );
        for( auto it = fragOrders.begin(); it != fragOrders.end(); ++it )
        {
            ADN_Missions_FragOrder* fragOrder = static_cast< ADN_Missions_FragOrder* >(*it);
            if( cit->second == fragOrder->strName_.GetData() )
            {
                pNewInfo->strName_   = fragOrder->strName_.GetData();
                pNewInfo->SetCrossedElement( fragOrder );
            }
        }
        ADN_Connector_Vector_ABC& pCList = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCList.AddItem( pNewInfo );
        if( ADN_StandardItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::RemoveCurrentItem
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::RemoveCurrentItem()
{
    // delete composante
    OrderInfos* pCurComposante = reinterpret_cast< OrderInfos* >( GetCurrentData() );
    if( pCurComposante )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurComposante );
    }
}
