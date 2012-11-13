// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "ADN_App.h"

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_TypePtr_InVector_ABC<T>::ADN_TypePtr_InVector_ABC()
    : pData_(0)
    , pVector_(0)
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_TypePtr_InVector_ABC<T>::ADN_TypePtr_InVector_ABC( const typename ADN_TypePtr_InVector_ABC::T_TypePtr& value )
    : pData_(0)
    , pVector_(0)
{
    SetData( value );
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_TypePtr_InVector_ABC<T>::ADN_TypePtr_InVector_ABC( typename ADN_TypePtr_InVector_ABC<T>::T_TypeVector& v, const typename ADN_TypePtr_InVector_ABC::T_TypePtr& value )
    : pData_(0)
    , pVector_(0)
{
    SetVector(v);
    if( value == 0 && ! v.empty() )
        SetData( v.front() );
    else
        SetData( value );
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_TypePtr_InVector_ABC<T>::~ADN_TypePtr_InVector_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::ConnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    connect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItem(void*)));
    connect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItem(void*)));
    connect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    connect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));

    pTarget->Initialize( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::DisconnectPrivateSub
// Created: APE 2005-02-28
// -----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );

    disconnect( pTarget, SIGNAL(ItemAdded(void*)),     this, SLOT(AddItem(void*)));
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)),   this, SLOT(RemItem(void*)));
    disconnect( pTarget, SIGNAL(ItemSwapped(int,int)), this, SLOT(SwapItem(int,int)));
    disconnect( pTarget, SIGNAL(Cleared(bool)),        this, SLOT(Clear(bool)));
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::SetData
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::SetData(const T_TypePtr& value )
{
    if( pData_!= value)
    {
        if( pData_ )
        {
            static_cast<ADN_Ref_ABC*>(pData_)->ADN_Ref_ABC::DisconnectPrivate( static_cast<ADN_Ref_ABC*>(this) );
            pData_->RemoveRef();
        }

        pData_    =value;
        emit DataChanged(pData_);

        if( pData_ )
        {
            static_cast<ADN_Ref_ABC*>(pData_)->ADN_Ref_ABC::ConnectPrivate( static_cast<ADN_Ref_ABC*>(this) );
            pData_->AddRef();
        }
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::T_TypePtr
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
typename const ADN_TypePtr_InVector_ABC<T>::T_TypePtr ADN_TypePtr_InVector_ABC<T>::GetData() const
{
    return pData_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::T_TypeVector
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
typename ADN_TypePtr_InVector_ABC<T>::T_TypeVector& ADN_TypePtr_InVector_ABC<T>::GetVector()
{
    return *pVector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::T_TypeVector
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::SetVector(const typename ADN_TypePtr_InVector_ABC<T>::T_TypeVector& v)
{
    if (pVector_!=&v)
    {
        // disconnect previous vector
        if (pVector_)
            static_cast<ADN_Connector_Vector_ABC*>(this)->Disconnect(static_cast<ADN_Connector_Vector_ABC*>(pVector_));

        // set new vector
        pVector_=(T_TypeVector*)&v;

        // connect new vector
        if (pVector_)
            static_cast<ADN_Connector_Vector_ABC*>(this)->Connect(static_cast<ADN_Connector_Vector_ABC*>(pVector_));

    }
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::operator =
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
template< class T >
ADN_TypePtr_InVector_ABC<T>& ADN_TypePtr_InVector_ABC<T>::operator =(const T_TypePtr& val)
{
    SetData( val );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::AddItemPrivate
/** @param  item
    @param  bInConnection
    @return
*/
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template< class T >
bool ADN_TypePtr_InVector_ABC< T >::AddItemPrivate( void* item )
{
    return item != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::RemItemPrivate
/** @param  item
    @return
*/
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
template< class T >
bool ADN_TypePtr_InVector_ABC< T >::RemItemPrivate( void* item )
{
    if( item == 0 )
        return false;
    if( pData_ == item )
        InvalidatePrivate( item );
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::operator ==
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
template< class T >
bool ADN_TypePtr_InVector_ABC<T>::operator ==(const ADN_TypePtr_InVector_ABC& o) const
{
    return pData_==o.pData_ && pVector_==o.pVector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::operator ==
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
template< class T >
bool ADN_TypePtr_InVector_ABC<T>::operator ==(const T_TypePtr& val) const
{
    return pData_==val;
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>::SetDataPrivate
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::SetDataPrivate( void *data )
{
    T_TypePtr newData = static_cast< T_TypePtr >( data );
    SetData( newData );
    ADN_App::GetMainWindow()->setWindowModified( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC<T>:: InvalidatePrivate
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC<T>::InvalidatePrivate(void *ptr,bool bDel)
{
    if( pData_ == ptr)
    {
        ADN_TypePtr_InVector_ABC<T>::SetData((T_TypePtr)0);
        ADN_Ref_ABC::InvalidatePrivate(this);
        emit Invalidated(ptr,bDel);
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::Initialize
/** @param  dest
*/
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC< T >::Initialize( ADN_Connector_Vector_ABC& dest ) const
{
    if( pVector_ != 0 )
    {
        dest.Clear( true );
        for (T_TypeVector::IT_PtrVector it=pVector_->begin();it!=pVector_->end();++it)
            dest.AddItemNoEmit( *it );
        dest.AddItemNoEmit( 0 );
    }
    dest.SetData( pData_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TypePtr_InVector_ABC::Initialize
// Created: APE 2005-01-06
// -----------------------------------------------------------------------------
template< class T >
void ADN_TypePtr_InVector_ABC< T >::Initialize( ADN_Connector_ABC& dest ) const
{
    dest.SetData( pData_ );
}
