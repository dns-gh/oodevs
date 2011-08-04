// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Ref_ABC.h"
#include "moc_ADN_Ref_ABC.cpp"

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC constructor
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
ADN_Ref_ABC::ADN_Ref_ABC( const char* szName )
    : QObject       ( 0, szName )
    , bConnecting_  ( false )
    , bSlotsBlocked_( false )
    , nRef_         ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC destructor
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
ADN_Ref_ABC::~ADN_Ref_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::Connect
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::Connect( ADN_Ref_ABC* pTarget, bool bConnect )
{
    if( bConnect )
        this->Connect( pTarget );
    else
        this->Disconnect( pTarget );
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::Connect
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::Connect( ADN_Ref_ABC* pTarget )
{
    pTarget->ConnectPrivate( this );
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::Disconnect
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::Disconnect( ADN_Ref_ABC* pTarget )
{
    pTarget->DisconnectPrivate( this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::ConnectPrivate( ADN_Ref_ABC* pTarget )
{
    pTarget->ConnectPrivateSub( this );
    this->ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::ConnectPrivate( ADN_Connector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::ConnectPrivate( ADN_Connector_Vector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::DisconnectPrivate( ADN_Ref_ABC* pTarget )
{
    pTarget->DisconnectPrivateSub( this );
    this->DisconnectPrivateSub( pTarget );
}


// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::DisconnectPrivate( ADN_Connector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::DisconnectPrivate( ADN_Connector_Vector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::ConnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::ConnectPrivateSub( ADN_Ref_ABC* pTarget )
{
    connect( pTarget ,SIGNAL(Invalidated(void *,bool)), this ,SLOT(Invalidate(void *,bool)));
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::DisconnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::DisconnectPrivateSub( ADN_Ref_ABC* pTarget )
{
    disconnect( pTarget ,SIGNAL(Invalidated(void *,bool)), this ,SLOT(Invalidate(void *,bool)));
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::BindExistenceTo
// Created: JDY 03-08-26
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::BindExistenceTo( const ADN_Ref_ABC* pObj )
{
    connect( pObj, SIGNAL( Invalidated( void*, bool ) ), this, SLOT( AutoDelete() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::Invalidate
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::Invalidate( void* pObj, bool bDel )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        InvalidatePrivate( pObj, bDel );
        BlockSlots( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::InvalidatePrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::InvalidatePrivate( void* pObj, bool bDel )
{
    // Warn others that we are invalidated.
    emit Invalidated( pObj, false );

    // Del ptr if necessary
    //$$$$ Unused ?
    if( bDel )
        delete pObj;
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::AutoCreate
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::AutoCreate( void* pObj )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        AutoCreatePrivate( pObj );
        BlockSlots( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::AutoCreatePrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::AutoCreatePrivate( void* /*pObj*/ )
{
}

//-----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::AutoDelete
// Created: JDY 03-08-26
//-----------------------------------------------------------------------------
void ADN_Ref_ABC::AutoDelete()
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );

        // Warn others before self deleting.
        emit Invalidated( this, false );

        // Self delete with a delay to prevent some weird Qt bug.
        QTimer::singleShot( 0, this, SLOT( AutoDeleteEx() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Ref_ABC::AutoDeleteEx
// Created: APE 2004-12-30
// -----------------------------------------------------------------------------
void ADN_Ref_ABC::AutoDeleteEx()
{
    delete this;
}
