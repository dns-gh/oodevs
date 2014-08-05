// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_Vector_ABC.h"
#include "moc_ADN_Connector_Vector_ABC.cpp"

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_Vector_ABC::ADN_Connector_Vector_ABC()
    : bConnecting_     ( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC destructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_Vector_ABC::~ADN_Connector_Vector_ABC()
{
    // NOTHING
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

    connect( pTarget, SIGNAL( ItemAdded( void* ) ),         this, SLOT( AddItem( void* ) ) );
    connect( pTarget, SIGNAL( ItemRemoved( void* ) ),       this, SLOT( RemItem( void*) ) );
    connect( pTarget, SIGNAL( Invalidated( void*, bool ) ), this, SLOT( Invalidate( void*, bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::DisconnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL( ItemAdded( void* ) ),         this, SLOT( AddItem( void* ) ) );
    disconnect( pTarget, SIGNAL( ItemRemoved( void* ) ),       this, SLOT( RemItem( void* ) ) );
    disconnect( pTarget, SIGNAL( Invalidated( void*, bool ) ), this, SLOT( Invalidate( void*, bool ) ) );
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
        AddItemPrivate( 0 );
        BlockSlots( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::AddItemsNoEmit
// Created: SLI 2014-08-05
// -----------------------------------------------------------------------------
void ADN_Connector_Vector_ABC::AddItemsNoEmit( const std::vector< void* >& pObjs )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        for( auto it = pObjs.begin(); it != pObjs.end(); ++it )
            AddItemPrivate( *it );
        AddItemPrivate( 0 );
        BlockSlots( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Vector_ABC::AddItemPrivate
// Created: AGN 2004-03-19
// -----------------------------------------------------------------------------
bool ADN_Connector_Vector_ABC::AddItemPrivate( void* )
{
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
bool ADN_Connector_Vector_ABC::RemItemPrivate( void* )
{
    // nothing
    return false;
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
void  ADN_Connector_Vector_ABC::ClearPrivate( bool )
{
    // NOTHING
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
