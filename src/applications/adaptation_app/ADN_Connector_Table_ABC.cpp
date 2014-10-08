//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Table_ABC.cpp $
// $Author: Ape $
// $Modtime: 31/03/05 10:26 $
// $Revision: 6 $
// $Workfile: ADN_Connector_Table_ABC.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Table_ABC.h"
#include "ADN_Workspace.h"

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_Table_ABC::ADN_Connector_Table_ABC( ADN_Table_ABC& tab )
    : tab_(tab)
    , bIsConnected_(false)
{
    if( tab_.IsAutoEnabled())
        tab_.SetEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_Table_ABC::~ADN_Connector_Table_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::ConnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Table_ABC::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));

    if( tab_.IsAutoEnabled() )
        tab_.SetEnabled( true );
    bIsConnected_=true;

    pTarget->Initialize( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::DisconnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Table_ABC::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));

    bIsConnected_=false;
    if( tab_.IsAutoEnabled() )
        tab_.SetEnabled( false );
    if( IsAutoClear() )
        Clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_Table_ABC::AddItemPrivate( void* obj )
{
    if( obj )
    {
        // add just item
        vDatas_.push_back( obj );
    }
    else
    {
        // end of the list of items -> build tab
        tab_.setNumRows( 0 );
        for( unsigned i = 0; i < vDatas_.size(); ++i )
            AddSubItems( i, vDatas_[ i ] );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
bool ADN_Connector_Table_ABC::RemItemPrivate( void* pItem )
{
    if( pItem == 0 )
        return false;

    // Remove it from the data list.
    std::vector<void*>::iterator it = std::find( vDatas_.begin(), vDatas_.end(), pItem );
    if( it == vDatas_.end() )
        return false;

    vDatas_.erase( it );

    // Remove it from the table (which can be user sorted, and thus not match the order of vData_
    tab_.RemoveItem( pItem );

    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::ClearPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Connector_Table_ABC::ClearPrivate(bool bInConnection)
{
    if( bInConnection)
    {
        vDatas_.clear();
    }
    else
    {
        tab_.setNumRows( 0 );
        vDatas_.clear();
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::SetDataPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void  ADN_Connector_Table_ABC::SetDataPrivate( void* /*data*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::AddSubItems
// Created: ABR 2012-10-19
// -----------------------------------------------------------------------------
void ADN_Connector_Table_ABC::AddSubItems( int nRow, void* pObj )
{
    assert( pObj );
    tab_.AddRow( nRow, pObj );
}
