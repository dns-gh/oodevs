// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ADN_Workspace.h"

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC constructor
// Created: JDY 03-06-25
//-----------------------------------------------------------------------------
template< class T >
ADN_Type_Vector_ABC<T>::ADN_Type_Vector_ABC( bool bAutoRef )
    : bAutoRef_( bAutoRef )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC constructor
// Created: JDY 03-09-02
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Vector_ABC<T>::ADN_Type_Vector_ABC( const ADN_Type_Vector_ABC& o )
    : std::vector<T*>( o )
    , bAutoRef_( o.bAutoRef_ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC destructor
// Created: JDY 03-06-25
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Vector_ABC<T>::~ADN_Type_Vector_ABC()
{
    for( auto it = checkers_.begin(); it != checkers_.end(); ++it )
        delete *it;
    checkers_.clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC operator =
// Created: JDY 03-09-02
//-----------------------------------------------------------------------------
template <class T>
ADN_Type_Vector_ABC<T>& ADN_Type_Vector_ABC<T>::operator =( const ADN_Type_Vector_ABC& o )
{
    std::vector<T*>::operator =(o);
    return *this;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::AddItemPrivate
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
template <class T>
bool ADN_Type_Vector_ABC<T>::AddItemPrivate( void* pItem )
{
    if( pItem == 0 )
        return true;

    T* pCastItem = static_cast< T* >( pItem );
    if( bAutoRef_ )
        connect( static_cast< ADN_Ref_ABC* >( pItem ), SIGNAL( Invalidated( void*, bool ) ), this, SLOT( Invalidate( void*, bool ) ) );

    push_back( pCastItem );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::RemItemPrivate
// Created: AGN 2004-05-11
// -----------------------------------------------------------------------------
template <class T>
bool ADN_Type_Vector_ABC< T >::RemItemPrivate( void* pItem )
{
    if( pItem == 0 )
        return true;

    T* pCastItem = static_cast< T* >( pItem );

    // rem item from vector
    auto it = std::find( begin(), end(), pCastItem );
    if( it == end() )
        return false;

    erase( it );

    // rem ref
    if( bAutoRef_ )
        disconnect( static_cast< ADN_Ref_ABC* >( pItem ), SIGNAL( Invalidated( void*, bool ) ), this, SLOT( Invalidate( void*, bool ) ) );
    
    ADN_App::GetMainWindow()->setWindowModified( true );
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::EndVector
// Created: JDY 03-07-21
//-----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::EndVector()
{
    AddItem( 0 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::SwapItemPrivate
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
template< class T >
void ADN_Type_Vector_ABC< T >::SwapItemPrivate( int i, int j )
{
    if( size() == 0 || i == j )
        return;
    else if( i < 0 )
        SwapItem( 0, j) ;
    else if( j >= ( int ) size() )
        SwapItem( i, ( int ) size() - 1 );
    else if( i > j )
        SwapItem( j , i );
    else
    {
        T* tmp = at( i );
        at( i ) = at( j );
        at( j ) = tmp;
        emit ItemSwapped( i, j );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::InvalidatePrivate
// Created: JDY 03-08-25
//-----------------------------------------------------------------------------
template < class T >
void ADN_Type_Vector_ABC<T>::InvalidatePrivate(void *item,bool bDel)
{
    auto it = std::find( begin(), end(), ( T* )item);
    if (it!= end() )
    {
        // rem item from vector
        // and invalidate it
        erase( it );
        emit ItemRemoved( item );
        emit Invalidated( item, false );

        // rem ref
        if( bAutoRef_ )
        {
            disconnect( static_cast< ADN_Ref_ABC* >( item ), SIGNAL( Invalidated( void*, bool ) ),
                        this,                              SLOT( Invalidate( void*, bool ) ) );

        }

        // delete item
        if( bDel )
            delete item;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::ClearPrivate
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::ClearPrivate(bool bInConnection)
{
    if( !bInConnection && size()!=0 )
    {
        clear();
        emit Cleared(bInConnection);
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::Reset
// Created: JDY 03-09-02
//-----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::Reset()
{
    if( empty() )
        return;

    // backup vector in order to delete ptr after
    // cleaning vector - ptr may be in use in Cleared
    // signal
    ADN_Type_Vector_ABC<T> vTmp=*this;

    // clear vector
    clear();
    emit Cleared(false);

    // delete ptrs
    clear_owned_ptrs(vTmp);
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::Delete
// Created: LDC 2010-09-13
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::Delete()
{
    ADN_Type_Vector_ABC<T>& vTmp=*this;
    clear_owned_ptrs( vTmp );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC<T>::SetDataPrivate
// Created: JDY 03-07-02
//-----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::SetDataPrivate( void* /*data*/ )
{
//    assert( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::Initialize
// Created: AGN 2004-03-22
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC<T>::Initialize( ADN_Connector_Vector_ABC& dest ) const
{
    dest.Clear( true );
    for( auto it = begin(); it != end(); ++it )
    {
        T* pObj = *it;
        dest.AddItemNoEmit( pObj );
    }
    dest.AddItemNoEmit( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::push_back
// Created: APE 2005-04-07
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC< T >::push_back(  T* const & x )
{
    std::vector<T*>::push_back( x );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::AddUniquenessChecker
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template< class T >
void ADN_Type_Vector_ABC< T >::AddUniquenessChecker( ADN_ErrorStatus errorType, const QString& errorMsg, T_Extractor extractor /* = &ADN_Tools::NameExtractor */ )
{
    checkers_.push_back( new UniquenessChecker( errorType, errorMsg, extractor ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::IsValid
// Created: ABR 2013-01-15
// -----------------------------------------------------------------------------
template <class T>
void ADN_Type_Vector_ABC< T >::CheckValidity()
{
    ADN_Ref_ABC::CheckValidity();
    ADN_ErrorStatus errorStatus = GetErrorStatus();

    // Elements validity
    for( auto it = begin(); it != end(); ++it )
    {
        ( *it )->CheckValidity();
        errorStatus = std::max< ADN_ErrorStatus >( ( *it )->GetErrorStatus(), errorStatus );
    }

    // Vector validity
    for( auto checker = checkers_.begin(); checker != checkers_.end(); ++checker )
    {
        assert( *checker != 0 );
        for( auto lhs = begin(); lhs != end(); ++lhs )
            for( auto rhs = lhs + 1; rhs != end(); ++rhs )
                if( lhs != rhs && !( *checker )->IsValid( **lhs, **rhs ) )
                {
                    ( *checker )->InvalidData( **lhs, **rhs );
                    errorStatus = std::max< ADN_ErrorStatus >( ( *checker )->status_, errorStatus );
                }
    }

    SetErrorStatus( errorStatus );
}

// -----------------------------------------------------------------------------
// Name: ADN_Type_Vector_ABC::FindElements
// Created: ABR 2013-04-23
// -----------------------------------------------------------------------------
template< typename T >
std::vector< T* > ADN_Type_Vector_ABC< T >::FindElements( boost::function< bool( const T* ) > func ) const
{
    std::vector< T* > result;
    for( auto it = begin(); it != end(); ++it )
        if( *it && func( *it ) )
            result.push_back( *it );
    return result;
}
