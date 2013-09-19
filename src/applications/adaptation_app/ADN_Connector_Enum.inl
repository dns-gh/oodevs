//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Enum.inl $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Enum.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum< T > constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_Enum< T >::ADN_Connector_Enum(T* pGfx)
: ADN_Connector_ABC()
, pGfx_(pGfx)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum< T > destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_Enum< T >::~ADN_Connector_Enum()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::AddEnumValue
// Created: ABR 2012-10-26
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Enum< T >::AddEnumValue( const std::string& text, int index )
{
    pGfx_->insertItem( text.c_str(), index );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::ConnectPrivateSub
// Created: ABR 2012-10-26
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Enum< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( pTarget );
    bIsConnected_ = true;
    if( pGfx_->IsAutoEnabled() )
        pGfx_->setEnabled( ShouldEnableGfx() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::DisconnectPrivateSub
// Created: ABR 2012-10-26
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Enum< T >::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( pTarget );
    bIsConnected_ = false;
    pGfx_->setCurrentItem(-1);
    pGfx_->clear();
    if( pGfx_->IsAutoEnabled() )
        pGfx_->setEnabled( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum< T >::ClearPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Enum< T >::ClearPrivate(bool bInConnection )
{
    if( !bInConnection )
    {
        pGfx_->setCurrentItem(-1);
        pGfx_->clear();
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum< T >::SetNdxChanged
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void* ADN_Connector_Enum< T >::SetNdxChanged( int ndx )
{
    emit DataChanged(&ndx);
    if( ndx == -1 )
        return 0; // Clear
    else
        return this; // change value
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum< T >::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Enum< T >::SetDataPrivate( void* data)
{
    // current selected data changed
    int ndx=*(int*)data;
    if( ndx!=-1)
        pGfx_->setCurrentItem(ndx);
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::IsConnected
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template< class T >
bool ADN_Connector_Enum< T >::IsConnected() const
{
    return bIsConnected_;
}
