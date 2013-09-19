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

namespace
{
    QString& removeTrailingZero( QString& input, QChar decimalPoint )
    {
        int i = input.find( decimalPoint );
        if( i >= 0 )
        {
            int j = input.size() - 1;
            while( j > i && input[ j ] == '0' ){ --j; }
            input.truncate( j == i ? j : j + 1 );
        }
        return input;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::SetDataPrivate
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void ADN_Connector_Double<T>::SetDataPrivate(void *data)
{
    assert(data);
    QLocale locale;
    int decimals = 3;
    if( pGfx_ && pGfx_->validator() )
        if( const QDoubleValidator* validator = dynamic_cast< const QDoubleValidator* >( pGfx_->validator() ) )
            decimals = validator->decimals();
    QString strText = locale.toString( *(double*)data, 'f', decimals );
    strText = removeTrailingZero( strText, locale.decimalPoint() );
    if( strText != pGfx_->text() )
    {
        int pos = pGfx_->cursorPosition();
        pGfx_->setText(strText);
        pGfx_->setCursorPosition( pos );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::SetDataChanged
// Created: JDY 03-07-08
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Double<T>::SetDataChanged(const QString& string)
{
    QLocale locale;
    bool ok = false;
    double newval = locale.toDouble( string, &ok );
    if( !ok )
        newval = string.toDouble( &ok );
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
        pGfx_->setEnabled( ShouldEnableGfx() );
    bIsConnected_ = true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Double<T>::disconnectNotify
// Created: JDY 03-07-16
//-----------------------------------------------------------------------------
template <class T>
void  ADN_Connector_Double<T>::disconnectNotify(const char *signal)
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

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Double::ConnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Double< T >::ConnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    connect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::ConnectPrivateSub( pTarget );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Double::DisconnectPrivateSub
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Double< T >::DisconnectPrivateSub( ADN_Connector_ABC* pTarget )
{
    disconnect( pTarget, SIGNAL( SendErrorStatus( ADN_ErrorStatus, const QString& ) ), pGfx_, SLOT( Warn( ADN_ErrorStatus, const QString& ) ) );
    ADN_Connector_ABC::DisconnectPrivateSub( pTarget );
}
