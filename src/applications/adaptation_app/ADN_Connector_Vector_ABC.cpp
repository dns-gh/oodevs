//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Vector_ABC.cpp $
// $Author: Ape $
// $Modtime: 3/03/05 15:47 $
// $Revision: 6 $
// $Workfile: ADN_Connector_Vector_ABC.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Connector_Vector_ABC.h"
#include "moc_ADN_Connector_Vector_ABC.cpp"


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_Vector_ABC::ADN_Connector_Vector_ABC( const char* szName )
: ADN_Connector_ABC( szName )
, bConnecting_     ( false )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC destructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_Vector_ABC::~ADN_Connector_Vector_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Connect
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::Connect( ADN_Ref_ABC* pTarget, bool bConnect )
{
    if( bConnect )
        this->Connect( pTarget );
    else
        this->Disconnect( pTarget );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Connect
// Created: APE 2005-02-28
//-----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::Connect( ADN_Ref_ABC* pTarget )
{
    pTarget->ConnectPrivate( this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Disconnect
// Created: APE 2005-02-28
//-----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::Disconnect( ADN_Ref_ABC* pTarget )
{
    pTarget->DisconnectPrivate( this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::ConnectPrivate( ADN_Ref_ABC* /*pTarget*/ )
{
    assert( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::ConnectPrivate( ADN_Connector_ABC* /*pTarget*/ )
{
    assert( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::ConnectPrivate( ADN_Connector_Vector_ABC* pTarget )
{
    pTarget->ConnectPrivateSub( this );
    this->ConnectPrivateSub( pTarget );
}



// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::DisconnectPrivate( ADN_Ref_ABC* /*pTarget*/ )
{
    assert( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::DisconnectPrivate( ADN_Connector_ABC* /*pTarget*/ )
{
    assert( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::DisconnectPrivate( ADN_Connector_Vector_ABC* pTarget )
{
    pTarget->DisconnectPrivateSub( this );
    this->DisconnectPrivateSub( pTarget );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::ConnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItem(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItem(void*)));
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::DisconnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItem(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItem(void*)));
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::AddItem
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::AddItem( void* pObj )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        if( AddItemPrivate( pObj ) )
            emit ItemAdded( pObj );
        BlockSlots( false );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::AddItemNoEmit
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::AddItemNoEmit( void* pObj )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        AddItemPrivate( pObj );
        BlockSlots( false );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_Vector_ABC::AddItemPrivate( void*, bool )
{
    // nothing
    return false;
}



// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::RemItem
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::RemItem( void* pObj )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        if( RemItemPrivate( pObj ) )
            emit ItemRemoved( pObj );
        BlockSlots( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::RemItemNoEmit
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::RemItemNoEmit( void* pObj )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        RemItemPrivate( pObj );
        BlockSlots( false );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
bool ADN_Connector_Vector_ABC::RemItemPrivate( void*, bool )
{
    // nothing
    return false;
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::SwapItem
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::SwapItem( int i, int j )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        SwapItemPrivate( i, j );
        BlockSlots( false );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::SwapItemPrivate
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::SwapItemPrivate( int i, int j )
{
    emit ItemSwapped( i, j );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Sort
// Created: APE 2005-01-14
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::Sort( ADN_BinaryPredicateWrapper& lessComp )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        SortPrivate( lessComp );
        BlockSlots( false );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::SortPrivate
// Created: APE 2005-01-14
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::SortPrivate( ADN_BinaryPredicateWrapper& lessComp )
{
    emit Sorted( lessComp );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Clear
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::Clear( bool bInConnection )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        ClearPrivate( bInConnection );
        BlockSlots( false );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::ClearPrivate
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::ClearPrivate( bool bInConnection )
{
    emit Cleared( bInConnection );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::SetDataPrivate
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void  ADN_Connector_Vector_ABC::SetDataPrivate( void* pData )
{
    emit DataChanged( pData );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::Initialize
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::Initialize( ADN_Connector_Vector_ABC& ) const
{
    // nothing
}

