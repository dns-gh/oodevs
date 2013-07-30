// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC constructor
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template< class T, class Cmp >
ADN_Type_VectorFixed_ABC< T, Cmp >::ADN_Type_VectorFixed_ABC()
    : ADN_Type_Vector_ABC< T >( true )
    , v_( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC destructor
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template< class T, class Cmp >
ADN_Type_VectorFixed_ABC< T, Cmp >::~ADN_Type_VectorFixed_ABC()
{
    ResetFixedVector();
    clear_owned_ptrs( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_Type_VectorFixed_ABC::AutoCreatePrivate
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
template< class T, class Cmp >
void ADN_Type_VectorFixed_ABC< T, Cmp >::AutoCreatePrivate( void* ptr )
{
    if( ptr && std::find_if( begin(), end(), Cmp( ( T_Item* ) ptr ) ) == end() )
    {
        T* pNewItem = new T( ( T_Item* ) ptr );
        AddItemPrivate( pNewItem );
        EndVector();
        emit ItemAdded( pNewItem );
    }
}
