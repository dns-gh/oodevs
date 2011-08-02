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

#include "ADN_App.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Models_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Tools.h"
#include "ENT/ENT_Tr.h"

#include <Qt3Support/q3header.h>
#include <Qt3Support/q3popupmenu.h>

typedef ADN_Models_Data::OrderInfos OrderInfos;

//-----------------------------------------------------------------------------
// Internal List View connector
//-----------------------------------------------------------------------------
class ADN_CLV_Orders
: public ADN_Connector_ListView_ABC
{

public:

    ADN_CLV_Orders(ADN_ListView_Orders& list)
    : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_Orders()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem = new ADN_ListViewItem(&list_,obj,1);

        // connect list item with object's name
        pItem->Connect(0,&static_cast<OrderInfos*>(obj)->strName_);
        return pItem;
    }
private:
    ADN_CLV_Orders& operator=( const ADN_CLV_Orders& );
};


// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders constructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::ADN_ListView_Orders( bool usedWithMission, QWidget * parent, const char * name , Qt::WFlags f)
:   ADN_ListView(parent,name,f)
, usedWithMission_ ( usedWithMission )
{
    // add one column
    addColumn( tr( "Frag orders"));
    setResizeMode(Q3ListView::AllColumns);

    // connector creation
    pConnector_=new ADN_CLV_Orders(*this);

    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders destructor
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
ADN_ListView_Orders::~ADN_ListView_Orders()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::OnContextMenu
// Created: AGN 2003-11-27
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Normal )
        return;

    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu(this) );

    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData() != 0;

    unsigned int n = 0;
    ADN_Missions_Data::T_FragOrder_Vector& fragOrders = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders();
    for( ADN_Missions_Data::IT_FragOrder_Vector it = fragOrders.begin(); it != fragOrders.end(); ++it )
    {
        if ( usedWithMission_ || (*it)->isAvailableWithoutMission_.GetData() )
        {
            std::string strOrderName = (*it)->strName_.GetData();
            const int id = pTargetMenu->insertItem( strOrderName.c_str(), 2 + n );
            const bool added = Contains( strOrderName );
            pTargetMenu->setItemEnabled( id, !added );
            pTargetMenu->setItemChecked( id, added );
            bDisplayAdd |= !added;
            ++n;
        }
    }
    if( ! bDisplayAdd && !bDisplayRem )
        return;

    Q3PopupMenu * pMenu=new Q3PopupMenu(this);
    if( bDisplayAdd )
        pMenu->insertItem( tr( "Add frag order"), pTargetMenu.get() ,0 );
    if( bDisplayRem )
        pMenu->insertItem( tr( "Remove frag order"), 1 );

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
    ADN_ListViewItem* pItem = static_cast< ADN_ListViewItem* >( firstChild() );
    while( pItem != 0 )
    {
        if( strComposanteName == pItem->text( 0 ).ascii() )
            return true;

        pItem = static_cast< ADN_ListViewItem* >( pItem->nextSibling() );
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
    OrderInfos* pNewInfo = new OrderInfos();
    if( ADN_Missions_Data::FragOrder* fragOrder = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders().at( n ) )
    {
        pNewInfo->strName_   = fragOrder->strName_.GetData();
        pNewInfo->fragOrder_ = fragOrder;
    }
    else
    {
        // $$$$ SBO 2006-12-04: commit suicide
    }

    ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCList->AddItem( pNewInfo );
    setCurrentItem(FindItem(pNewInfo));
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Orders::RemoveCurrentItem
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_ListView_Orders::RemoveCurrentItem()
{
    // delete composante
    OrderInfos* pCurComposante = (OrderInfos*)GetCurrentData();
    if( pCurComposante )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurComposante );
    }
}
