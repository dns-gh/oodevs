// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_VectorFixed_ABC_h_
#define __ADN_Type_VectorFixed_ABC_h_

#include "ADN_Type_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-08-28
//*****************************************************************************
template< class T, class Cmp = T::CmpRef >
class ADN_Type_VectorFixed_ABC : public ADN_Type_Vector_ABC< T >
{
    typedef typename T::T_Item T_Item;

public:
    explicit ADN_Type_VectorFixed_ABC( const char* szName = 0 );
    virtual ~ADN_Type_VectorFixed_ABC();

public:
    template< class U >
    explicit ADN_Type_VectorFixed_ABC( const ADN_Type_Vector_ABC< U >& v )
        : ADN_Type_Vector_ABC< T >()
    {
        SetFixedVector( v );
    }

    template< class U > void SetFixedVector( const ADN_Type_Vector_ABC< U >& v )
    {
        // initialize vector
        for( auto it = v.begin(); it != v.end(); ++it )
            AddItem( new T( *it ) );
        EndVector();
        // connection
        connect( &v, SIGNAL( ItemAdded( void* ) ), this, SLOT( AutoCreate( void* ) ) );
        connect( &v, SIGNAL( ItemSwapped( int, int ) ), this, SLOT( SwapItem( int, int ) ) );
        connect( this, SIGNAL( ItemSwapped( int, int ) ), &v, SLOT( SwapItem( int, int ) ) );
    }

protected:
    virtual void AutoCreatePrivate( void* ptr );
};

#include "ADN_Type_VectorFixed_ABC.inl"

#endif // __ADN_Type_VectorFixed_ABC_h_