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
#include "ADN_Workspace.h"

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_ListView_ABC::ADN_Connector_ListView_ABC( ADN_ListView& list )
    : ADN_Connector_Vector_ABC()
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
    connect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), &list_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );

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
    disconnect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), &list_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );

    bIsConnected_ = false;
    Clear();
    if( list_.IsAutoEnabled() )
        list_.setEnabled(false);
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_ListView_ABC::AddItemPrivate( void *obj )
{
    if( obj == 0 )
        return false;
    auto item = CreateItem( obj );
    if( !item )
        return false;
    list_.InsertItem( item );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
bool ADN_Connector_ListView_ABC::RemItemPrivate( void* item )
{
    if( item == 0 )
        return false;
    ADN_StandardItem *pItem = list_.FindItem( item );
    if( pItem )
        list_.TakeItem( pItem );
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
    return true;
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
        list_.Clear();
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ListView_ABC::SetDataPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void  ADN_Connector_ListView_ABC::SetDataPrivate( void* /*data*/ )
{
}
