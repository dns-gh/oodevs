// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String constructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_String< T >::ADN_Connector_String( T* gfx )
    : ADN_Connector_ABC()
    , pGfx_( gfx )
    , bIsConnected_( false )
{
    assert(pGfx_);
    if( pGfx_->IsAutoEnabled() )
        pGfx_->setEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String destructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_String< T >::~ADN_Connector_String()
{
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String< T >::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_String< T >::SetDataPrivate( void *data )
{
    assert( data );
    std::string* pTxt=(std::string*)data;
    if( pGfx_->text() != pTxt->c_str() )
        pGfx_->setText( ( ( std::string* ) data )->c_str() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String< T >::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template< class T >
void  ADN_Connector_String< T >::SetDataChanged( const QString& string )
{
    std::string newval = string.toStdString();
    emit DataChanged( ( void* ) &newval );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::connectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_String< T >::connectNotify( const char *signal )
{
    if (pGfx_->IsAutoEnabled() && signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
        pGfx_->setEnabled(ShouldEnableGfx());

    bIsConnected_ = true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::disconnectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_String< T >::disconnectNotify( const char *signal )
{
    if (signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
    {
        if( pGfx_->IsAutoEnabled() )
            pGfx_->setEnabled( false );
        if( IsAutoClear() )
            pGfx_->setText( "" );
    }
    bIsConnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_String::IsConnected
/** @return
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template< class T >
bool ADN_Connector_String< T >::IsConnected() const
{
    return bIsConnected_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_String::ConnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_String< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    connect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_String::DisconnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_String< T >::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    disconnect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::DisconnectPrivateSub( pTarget );
}
