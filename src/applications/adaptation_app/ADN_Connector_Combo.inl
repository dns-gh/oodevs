// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T> constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_Combo<T>::ADN_Connector_Combo(T* combo)
    : ADN_Connector_Combo_ABC()
    , pCombo_(combo)
    , bIsConnected_(false)
{
    assert(pCombo_);
    if (pCombo_->IsAutoEnabled())
        pCombo_->setEnabled(false);
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T> destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_Connector_Combo<T>::~ADN_Connector_Combo()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Combo::ConnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Combo<T>::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    if( pCombo_->IsAutoEnabled() )
        pCombo_->setEnabled(true);
    bIsConnected_=true;

    pTarget->Initialize( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Combo::DisconnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Combo<T>::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItemNoEmit(void*)));
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    bIsConnected_=false;

    Clear();
    if( pCombo_->IsAutoEnabled() )
        pCombo_->setEnabled(false);
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T>::AddItemPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
bool ADN_Connector_Combo<T>::AddItemPrivate( void* obj )
{
    if( obj == 0 )
        return false;

    pCombo_->insertItem(CreateItem(obj));
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Combo_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
template< class T >
bool ADN_Connector_Combo< T >::RemItemPrivate( void* item )
{
    if( item == 0 )
        return false;
    int ndx=pCombo_->FindNdx(item);
    if (ndx==-1)
        return false;

    pCombo_->removeItem(ndx);
    ADN_App::GetMainWindow()->setWindowModified( true );
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T>::ClearPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Combo<T>::ClearPrivate(bool bInConnection)
{
    if( bInConnection && bIsConnected_)
        return;
    pCombo_->SetCurrentData(0);
    pCombo_->clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T>::SetDataChanged
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void* ADN_Connector_Combo<T>::SetNdxChanged(int ndx)
{
    void * data=pCombo_->GetItem(ndx)->GetData();
    emit DataChanged(data);
    return data;
}

//-----------------------------------------------------------------------------
// Name: ADN_Connector_Combo<T>::SetDataPrivate
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_Connector_Combo<T>::SetDataPrivate(void *data)
{
    if (!data)
    {
        // data empty?? -> value other side not initialized
        // select automaticaly first elt of the combo if
        // there are elts
        if (pCombo_->count())
            SetNdxChanged(0);
        return;
    }

    // current selected data changed
    int ndx=pCombo_->FindNdx(data);
    if( ndx!=-1)
        pCombo_->setCurrentItem(ndx);
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Combo_ABC::IsConnected
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template< class T >
bool ADN_Connector_Combo< T >::IsConnected() const
{
    return bIsConnected_;
}
