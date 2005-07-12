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
// Name: ADN_Connector_Enum<T> constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
ADN_Connector_Enum<T>::ADN_Connector_Enum(T* pGfx)
: ADN_Connector_Vector_ABC()
, pGfx_(pGfx)
, nNb_(0)
, bIsConnected_( false )
{
    assert(pGfx_);
    if (pGfx_->IsAutoEnabled())
        pGfx_->setEnabled(false);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum<T> destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
ADN_Connector_Enum<T>::~ADN_Connector_Enum()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::ConnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template<class T>
void ADN_Connector_Enum<T>::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    bIsConnected_ = true;
    if( pGfx_->IsAutoEnabled() )
        pGfx_->setEnabled(true);    
    pTarget->Initialize( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::DisconnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template<class T>
void ADN_Connector_Enum<T>::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    bIsConnected_ = false;
    Clear();
    if( pGfx_->IsAutoEnabled() )
        pGfx_->setEnabled(false);
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum<T>::AddItemPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
bool ADN_Connector_Enum<T>::AddItemPrivate(void *obj,bool /*bInConnection*/)
{
    if( obj == 0 )
        return false;
    ++nNb_;
    pGfx_->insertItem(QString(GetItem(obj).c_str()));
    return true;
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
template<class T>
bool ADN_Connector_Enum< T >::RemItemPrivate(void *item)
{
    if( item == 0 )
        return false;
    --nNb_;
    pGfx_->removeItem(*(int*)item);
    return true;
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum<T>::ClearPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
void ADN_Connector_Enum<T>::ClearPrivate(bool bInConnection )
{
    if (!bInConnection)
    {
        pGfx_->setCurrentItem(-1);
        pGfx_->clear();
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum<T>::SetNdxChanged
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
void* ADN_Connector_Enum<T>::SetNdxChanged(int ndx)
{
    emit DataChanged(&ndx);
    if( ndx == -1 )
        return 0; // Clear
    else
        return this; // change value
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Enum<T>::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template<class T>
void ADN_Connector_Enum<T>::SetDataPrivate(void *data)
{
    // current selected data changed
    int ndx=*(int*)data;
    if ( ndx!=-1)
        pGfx_->setCurrentItem(ndx);
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Enum::IsConnected
/** @return 
*/
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template<class T>
bool ADN_Connector_Enum< T >::IsConnected() const
{
    return bIsConnected_;
}
