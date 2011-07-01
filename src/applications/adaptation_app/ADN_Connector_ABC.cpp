// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_ABC.h"
#include "moc_ADN_Connector_ABC.cpp"

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC constructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_ABC::ADN_Connector_ABC( const char* szName )
    : ADN_Ref_ABC ( szName )
    , bConnecting_( false )
    , bAutoClear_ ( false )
    , bAutoHide_  ( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC destructor
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
ADN_Connector_ABC::~ADN_Connector_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::Connect
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::Connect( ADN_Ref_ABC* pTarget, bool bConnect )
{
    if( bConnect )
        this->Connect( pTarget );
    else
        this->Disconnect( pTarget );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::Connect
// Created: APE 2005-02-28
//-----------------------------------------------------------------------------
void ADN_Connector_ABC::Connect( ADN_Ref_ABC* pTarget )
{
    pTarget->ConnectPrivate( this );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::Disconnect
// Created: APE 2005-02-28
//-----------------------------------------------------------------------------
void ADN_Connector_ABC::Disconnect( ADN_Ref_ABC* pTarget )
{
    pTarget->DisconnectPrivate( this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::ConnectPrivate( ADN_Ref_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::ConnectPrivate( ADN_Connector_ABC* pTarget )
{
    pTarget->ConnectPrivateSub( this );
    this->ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::ConnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::ConnectPrivate( ADN_Connector_Vector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::DisconnectPrivate( ADN_Ref_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::DisconnectPrivate( ADN_Connector_ABC* pTarget )
{
    pTarget->DisconnectPrivateSub( this );
    this->DisconnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::DisconnectPrivate
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::DisconnectPrivate( ADN_Connector_Vector_ABC* /*pTarget*/ )
{
    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::ConnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    ADN_Ref_ABC::ConnectPrivateSub( (ADN_Ref_ABC*)pTarget );

    pTarget->Initialize( *this );
    connect( pTarget, SIGNAL( DataChanged( void* ) ), this, SLOT( SetData( void* ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::DisconnectPrivateSub
// Created: APE 2005-03-03
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    ADN_Ref_ABC::DisconnectPrivate( (ADN_Ref_ABC*)pTarget );

    disconnect( pTarget, SIGNAL( DataChanged( void* ) ), this, SLOT( SetData( void* ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::Initialize
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::Initialize( ADN_Connector_ABC& /*dest*/ ) const
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_ABC::SetData
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
void ADN_Connector_ABC::SetData( void* pData )
{
    if( ! SlotsBlocked() )
    {
        BlockSlots( true );
        SetDataPrivate( pData );
        BlockSlots( false );
    }
}
