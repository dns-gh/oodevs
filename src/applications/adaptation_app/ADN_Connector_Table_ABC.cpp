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
#include "ADN_TableItem_ABC.h"

class DataComparison : public ADN_BinaryPredicate
{
public:
    DataComparison( const ADN_Connector_Table_ABC& tab ) : ADN_BinaryPredicate(), tab_( tab ) {};
    bool operator()( void* pL, void* pR ) const
    {
        return tab_.LessComparison( pL, pR );
    }

    const DataComparison& operator=( const DataComparison& );

    const ADN_Connector_Table_ABC& tab_;
};


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Connector_Table_ABC::ADN_Connector_Table_ABC(ADN_Table& tab, bool bWithSort, const char* szName )
: ADN_Connector_Vector_ABC( szName )
, tab_(tab)
, bIsConnected_(false)
, bWithSort_( bWithSort )
{
    if( tab_.IsAutoEnabled())
        tab_.setEnabled(false);
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
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));
    connect( pTarget, SIGNAL(Sorted( ADN_BinaryPredicateWrapper&)), this, SLOT(Sort(ADN_BinaryPredicateWrapper&)));

    if( tab_.IsAutoEnabled() )
        tab_.setEnabled(true);
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
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));
    disconnect( pTarget, SIGNAL(Sorted( ADN_BinaryPredicateWrapper&)), this, SLOT(Sort(ADN_BinaryPredicateWrapper&)));

    bIsConnected_=false;
    if( tab_.IsAutoEnabled() )
        tab_.setEnabled(false);
    if( IsAutoClear() )
        Clear();
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_Table_ABC::AddItemPrivate(void *obj,bool)
{
    if( obj)
    {
        // add just item
        vDatas_.push_back(obj);
    }
    else
    {
        tab_.setUpdatesEnabled( true );
        if( bWithSort_ )
        {
            DataComparison comparor( *this );
            ADN_BinaryPredicateWrapper wrapper( comparor );
            std::sort( vDatas_.begin(), vDatas_.end(), wrapper );
            emit Sorted( wrapper );
        }

        // end of the list of items -> build tab
        if( (size_t)tab_.numRows()!=vDatas_.size())
            tab_.setNumRows(static_cast< int >(vDatas_.size()));
        int i=0;
        for( std::vector<void*>::iterator it=vDatas_.begin();it!=vDatas_.end();++it,++i)
            AddSubItems(i,*it);
        tab_.EnableRefreshing(true);
        tab_.repaintContents(false);
    }

    return true;
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
bool ADN_Connector_Table_ABC::RemItemPrivate( void* pItem, bool )
{
    if( pItem == 0 )
        return false;

    // Remove it from the data list.
    std::vector<void*>::iterator it = std::find( vDatas_.begin(), vDatas_.end(), pItem );
    if( it == vDatas_.end() )
        return false;

    vDatas_.erase( it );

    // Remove it from the table (which can be user sorted, and thus not match the order
    // of vData_
    int n = 0;
    while( tab_.item( n, 0 ) != 0 )
    {
        ADN_TableItem_ABC* pTableItem = static_cast<ADN_TableItem_ABC*>( tab_.item( n, 0 ) );
        if( pTableItem->GetData() == pItem )
        {
            tab_.removeRow( n );
            break;
        }
        ++n;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::LessComparison
// Created: AGN 2003-10-30
// -----------------------------------------------------------------------------
bool ADN_Connector_Table_ABC::LessComparison( void* , void* ) const
{
    assert( false );
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
        tab_.StopEditing();
        tab_.EnableRefreshing(false);
        vDatas_.clear();
    }
    else
    {
        tab_.setNumRows(0);
        vDatas_.clear();
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Table_ABC::SetDataPrivate
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void  ADN_Connector_Table_ABC::SetDataPrivate( void* /*data*/ )
{
}
