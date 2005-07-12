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

#include "ADN_pch.h"
#include "ADN_ListView_Orders.h"

#include "ADN_App.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Models_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Tools.h"
#include "ENT/ENT_Tr.h"

#include <qheader.h>
#include <qpopmenu.h>

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
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem(&list_,obj,1);

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
ADN_ListView_Orders::ADN_ListView_Orders(QWidget * parent /*= 0*/, const char * name , WFlags f )
:   ADN_ListView(parent,name,f)
{
    // add one column
    addColumn( tr( "Orders"));
    setResizeMode(QListView::AllColumns);

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
    std::auto_ptr< QPopupMenu > pTargetMenu( new QPopupMenu(this) );

    
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData()!=0;
    
    for( int n = 0; n < eNbrFragOrder; ++n )
    {
        std::string strOrderName = ENT_Tr::ConvertFromFragOrder( (E_FragOrder)n, ENT_Tr_ABC::eToTr );
        if( Contains( strOrderName ) )
            continue;

        bDisplayAdd = true;
        pTargetMenu->insertItem( strOrderName.c_str(), 2 + n );
    }
    if( ! bDisplayAdd && !bDisplayRem )
        return;

    QPopupMenu * pMenu=new QPopupMenu(this);
    if ( bDisplayAdd )
        pMenu->insertItem( tr( "Add order"), pTargetMenu.get() ,0 );
    if ( bDisplayRem )
        pMenu->insertItem( tr( "Remove order"), 1 );
    
    int nMenu=pMenu->exec(pt);
    if ( nMenu == 1 )
    {
        RemoveCurrentItem();
    }
    else if ( nMenu > 1 )
    {
        assert( nMenu - 2< eNbrFragOrder );
        CreateNewItem( nMenu - 2 );
    }
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
    // Get the list of the defined targets
    OrderInfos* pNewInfo = new OrderInfos();
    pNewInfo->nOrderType_ = (E_FragOrder)(n);
    pNewInfo->strName_ = ENT_Tr::ConvertFromFragOrder( (E_FragOrder)(n), ENT_Tr_ABC::eToTr );

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
    OrderInfos* pCurComposante=(OrderInfos*)GetCurrentData();
    if ( pCurComposante )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurComposante);
    }    
}
