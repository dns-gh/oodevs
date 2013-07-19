// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString constructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
ADN_Connector_LocalizedString< T >::ADN_Connector_LocalizedString( T* gfx )
    : ADN_Connector_String< T >( gfx )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString destructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
ADN_Connector_LocalizedString< T >::~ADN_Connector_LocalizedString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString::SetDataPrivate
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Connector_LocalizedString< T >::SetDataPrivate( void* data )
{
    assert(data);
    std::string* pTxt=(std::string*)data;
    if( pGfx_->text()!=pTxt->c_str() )
        pGfx_->setText(((std::string*)data)->c_str());
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString::ConnectPrivateSub
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Connector_LocalizedString< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    connect( pTarget, SIGNAL( TypeChanged( int ) ), pGfx_, SIGNAL( TypeChanged( int ) ) );
    connect( pGfx_, SIGNAL( OnTypeChanged( int ) ), pTarget, SLOT( OnTypeChanged( int ) ) );
    ADN_Connector_String< T >::ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString::DisconnectPrivateSub
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Connector_LocalizedString< T >::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    disconnect( pTarget, SIGNAL( TypeChanged( int ) ), pGfx_, SIGNAL( TypeChanged( int ) ) );
    disconnect( pGfx_, SIGNAL( OnTypeChanged( int ) ), pTarget, SLOT( OnTypeChanged( int ) ) );
    ADN_Connector_String< T >::DisconnectPrivateSub( pTarget );
}
