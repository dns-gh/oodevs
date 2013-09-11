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
    explicit ADN_Type_VectorFixed_ABC();
    virtual ~ADN_Type_VectorFixed_ABC();

public:
    template< class U >
    explicit ADN_Type_VectorFixed_ABC( const ADN_Type_Vector_ABC< U >& v )
        : ADN_Type_Vector_ABC< T >()
        , v_( 0 )
    {
        SetFixedVector( v );
    }

    template< class U > void SetFixedVector( const ADN_Type_Vector_ABC< U >& v )
    {
        v_ = &v;
        // initialize vector
        for( auto it = v.begin(); it != v.end(); ++it )
            AddItem( new T( *it ) );
        EndVector();
        // connection
        connect( &v, SIGNAL( ItemAdded( void* ) ), this, SLOT( AutoCreate( void* ) ) );
    }

    void ResetFixedVector()
    {
        if( v_ )
        {
            disconnect( v_, SIGNAL( ItemAdded( void* ) ), this, SLOT( AutoCreate( void* ) ) );
            Reset();
            v_ = 0;
        }
    }

protected:
    virtual void AutoCreatePrivate( void* ptr );

private:
    const ADN_Connector_Vector_ABC* v_;
};

#include "ADN_Type_VectorFixed_ABC.inl"

#endif // __ADN_Type_VectorFixed_ABC_h_