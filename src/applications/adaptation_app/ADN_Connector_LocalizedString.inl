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
    , pTarget_( nullptr )
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
    pGfx_->setEnabled( ShouldEnableGfx() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString::ConnectPrivateSub
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Connector_LocalizedString< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    pTarget_ = static_cast< ADN_Type_LocalizedString* >( pTarget );
    assert( pTarget_ != 0 );
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
    pTarget_ = 0;
    disconnect( pTarget, SIGNAL( TypeChanged( int ) ), pGfx_, SIGNAL( TypeChanged( int ) ) );
    disconnect( pGfx_, SIGNAL( OnTypeChanged( int ) ), pTarget, SLOT( OnTypeChanged( int ) ) );
    ADN_Connector_String< T >::DisconnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_LocalizedString::ShouldEnableGfx
// Created: ABR 2013-08-29
// -----------------------------------------------------------------------------
template< typename T >
bool ADN_Connector_LocalizedString< T >::ShouldEnableGfx() const
{
    return kernel::Language::IsCurrentDefault() || ( pTarget_ && !pTarget_->GetKey().empty() );
}
