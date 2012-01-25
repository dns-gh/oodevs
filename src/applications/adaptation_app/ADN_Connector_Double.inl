//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Double.inl $
// $Author: Ape $
// $Modtime: 25/03/05 15:36 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Double.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double constructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Double<T>::ADN_Connector_Double(T* gfx)
: ADN_Connector_ABC()
, pGfx_(gfx)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double destructor
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
ADN_Connector_Double<T>::~ADN_Connector_Double()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void ADN_Connector_Double<T>::SetDataPrivate(void *data)
{
    assert(data);
//    char   istring[256];
//    sprintf(istring,"%f",*(double*)data);
    QString strText = QString::number( *(double*)data, 'g', 10 );
    if ( strText != pGfx_->text() )
        pGfx_->setText(strText);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Double<T>::SetDataChanged(const QString& string)
{
    double newval = string.toDouble();
    emit DataChanged( ( void* ) &newval );
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::connectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Double<T>::connectNotify(const char *signal)
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
void  ADN_Connector_Double<T>::disconnectNotify(const char *signal)
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
// Name: ADN_Connector_Double::IsConnected
/** @return 
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class T>
bool ADN_Connector_Double< T >::IsConnected() const
{
    return bIsConnected_;
}
