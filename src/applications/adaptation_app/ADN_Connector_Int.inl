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
    if( strcmp( istring, pGfx_->text().toStdString().c_str() ) )
    {
        int pos = pGfx_->cursorPosition();
        pGfx_->setText(istring);
        pGfx_->setCursorPosition( pos );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Int<T>::SetDataChanged(const QString& string)
{
    QLocale locale;
    bool ok = false;
    int newval = locale.toInt( string, &ok );
    if( !ok )
        newval = string.toInt( &ok );
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
        pGfx_->setEnabled( ShouldEnableGfx() );
    bIsConnected_ = true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Int<T>::disconnectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Int<T>::disconnectNotify(const char *signal)
{
    if( signal != 0 && !strcmp(signal,SIGNAL(DataChanged(void*))))
    {
        if( pGfx_->IsAutoEnabled() )
            pGfx_->setEnabled(false);
        if( IsAutoClear() )
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

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Int::ConnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Int< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    connect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Int::DisconnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Int< T >::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    disconnect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::DisconnectPrivateSub( pTarget );
}
