//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_String.inl $
// $Author: Ape $
// $Modtime: 16/12/04 10:15 $
// $Revision: 2 $
// $Workfile: ADN_Connector_String.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String constructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_String<T>::ADN_Connector_String(T* gfx)
: ADN_Connector_ABC()
, pGfx_(gfx)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String destructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_String<T>::~ADN_Connector_String()
{
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String<T>::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void ADN_Connector_String<T>::SetDataPrivate(void *data)
{
    assert(data);
    std::string* pTxt=(std::string*)data;
    if( pGfx_->text()!=pTxt->c_str() )
        pGfx_->setText(((std::string*)data)->c_str());
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_String<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_String<T>::SetDataChanged(const QString& string)
{
    std::string newval=string.toStdString();
    emit DataChanged((void*)&newval);
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::connectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_String<T>::connectNotify(const char *signal)
{
    if (pGfx_->IsAutoEnabled() && signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
        pGfx_->setEnabled(true);

    bIsConnected_ = true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::disconnectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_String<T>::disconnectNotify(const char *signal)
{
    if (signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
    {
        if( pGfx_->IsAutoEnabled() )
            pGfx_->setEnabled(false);
        if( IsAutoClear() )
            pGfx_->setText("");
    }

    bIsConnected_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_String::IsConnected
/** @return
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class T>
bool ADN_Connector_String< T >::IsConnected() const
{
    return bIsConnected_;
}
