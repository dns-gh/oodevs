//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Bool.inl $
// $Author: Ape $
// $Modtime: 28/02/05 16:22 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Bool.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Bool constructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Bool<T>::ADN_Connector_Bool(T* gfx)
: ADN_Connector_ABC()
, pGfx_(gfx)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
    
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Bool destructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Bool<T>::~ADN_Connector_Bool()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Bool<T>::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void ADN_Connector_Bool<T>::SetDataPrivate(void *data)
{
    assert(data);
    pGfx_->setHidden( false );
    pGfx_->setChecked(*(bool*)data);
    if( bAutoHide_ && !*(bool*)data )
        pGfx_->setHidden( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Bool<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Bool<T>::SetDataChanged(bool b)
{
     emit DataChanged((void*)&b);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::connectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Bool<T>::connectNotify(const char *signal)
{
    if (pGfx_->IsAutoEnabled() && signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
        pGfx_->setEnabled(true);
    bIsConnected_ = true;
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::disconnectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Bool<T>::disconnectNotify(const char *signal)
{
    if ( signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
    {
        if ( pGfx_->IsAutoEnabled() )
            pGfx_->setEnabled(false);
        if ( IsAutoClear() )
            pGfx_->setChecked(false);
        bIsConnected_ = false;
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Bool::IsConnected
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class T>
bool ADN_Connector_Bool< T >::IsConnected() const
{
    return bIsConnected_;
}
