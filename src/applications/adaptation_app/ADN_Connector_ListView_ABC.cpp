//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_ListView_ABC.cpp $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 6 $
// $Workfile: ADN_Connector_ListView_ABC.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_ListView_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_ListView_ABC::ADN_Connector_ListView_ABC( ADN_ListView& list, const char* szName )
: ADN_Connector_Vector_ABC( szName )
, list_(list)
, bSwap_(false)
, bIsConnected_(false)
{
    if( list_.IsAutoEnabled())
        list_.setEnabled(false);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_ListView_ABC::~ADN_Connector_ListView_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::ConnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ListView_ABC::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    bIsConnected_ = true;
    if( list_.IsAutoEnabled() )
        list_.setEnabled(true);

    pTarget->Initialize( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::DisconnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ListView_ABC::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    bIsConnected_ = false;
    if( list_.IsAutoEnabled() )
    {
        Clear();
        list_.setEnabled(false);
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_ListView_ABC::AddItemPrivate(void *obj,bool)
{
    if( obj == 0 )
        return false;

    list_.insertItem(CreateItem(obj));
    return true;
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
bool ADN_Connector_ListView_ABC::RemItemPrivate(void *item, bool)
{
    if( item == 0 )
        return false;

    ADN_ListViewItem *pItem=list_.FindItem(item);
    if( pItem )
    {
        list_.takeItem(pItem);
    }
    return true;
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::SwapItemPrivate
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Connector_ListView_ABC::SwapItemPrivate(int i,int j)
{
    if( list_.childCount()==0 || i==j)
        return;
    else if( i < 0 )
        SwapItem(0,j);
    else if( j>=(int)list_.childCount())
        SwapItem(i,list_.childCount()-1);
    else if( i>j)
        SwapItem(j,i);
    else if( !bSwap_)
    {
        ADN_ListViewItem* pItemJ =list_.ItemAt(j);
        ADN_ListViewItem* pItemI =list_.ItemAt(i);

        if( i == 0)
        {
            // special case cause itemAbove of first Item
            // doesn't work
            if( j == 1)
                pItemI->moveItem(pItemJ);
            else
            {
                pItemI->moveItem(pItemJ->itemAbove());
                list_.takeItem(pItemJ);
                list_.insertItem(pItemJ);
            }
        }
        else
        {
            // general case
            ADN_ListViewItem* pAboveI=(ADN_ListViewItem*)pItemI->itemAbove();
            pItemI->moveItem(pItemJ->itemAbove());
            pItemJ->moveItem(pAboveI);
        }

        bSwap_=true;
        emit ItemSwapped(i,j);
        bSwap_=false;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::ClearPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Connector_ListView_ABC::ClearPrivate(bool bInConnection )
{
    if( bInConnection && bIsConnected_ )
        return;

    if( !bInConnection)
    {
        list_.SetCurrentItem((void*)0);
        list_.clear();
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::SetDataPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void  ADN_Connector_ListView_ABC::SetDataPrivate( void* /*data*/ )
{
}
