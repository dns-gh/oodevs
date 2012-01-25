//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Int.inl $
// $Author: Ape $
// $Modtime: 16/12/04 10:12 $
// $Revision: 2 $
// $Workfile: ADN_Connector_Int.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int constructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Int<T>::ADN_Connector_Int(T* gfx)
: ADN_Connector_ABC()
, pGfx_(gfx)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int destructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Int<T>::~ADN_Connector_Int()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void ADN_Connector_Int<T>::SetDataPrivate(void *data)
{
    assert(data);
    char   istring[256];
    sprintf_s(istring,"%d",*(int*)data);
    if ( strcmp(istring,pGfx_->text().ascii()) )
        pGfx_->setText(istring);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Int<T>::SetDataChanged(const QString& string)
{
    int newval = string.toInt();
    emit DataChanged( ( void* ) &newval );
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::connectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Int<T>::connectNotify(const char *signal)
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
void  ADN_Connector_Int<T>::disconnectNotify(const char *signal)
{
    if ( signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
    {
        if ( pGfx_->IsAutoEnabled() )
            pGfx_->setEnabled(false);
        if ( IsAutoClear() )
            pGfx_->setText("");
    }
    bIsConnected_ = false;
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Int::IsConnected
/** @return 
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class T>
bool ADN_Connector_Int< T >::IsConnected() const
{
    return bIsConnected_;
}
