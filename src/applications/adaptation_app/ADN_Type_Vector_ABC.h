// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Type_Vector_ABC_h_
#define __ADN_Type_Vector_ABC_h_

#include "ADN_Connector_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-06-26
//*****************************************************************************
template < class T >
class ADN_Type_Vector_ABC : public ADN_Connector_Vector_ABC
                          , public std::vector< T* >
{

public:
    typedef std::vector<T*>                T_PtrVector;
    typedef T_PtrVector::iterator          IT_PtrVector;
    typedef T_PtrVector::const_iterator    CIT_PtrVector;

public:
    explicit ADN_Type_Vector_ABC( bool bAutoRef = true );
    virtual ~ADN_Type_Vector_ABC();

    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;

    virtual void EndVector();
            void Reset();
            void Delete();

protected:
    virtual void SetDataPrivate( void* pData );
    virtual bool AddItemPrivate( void* pObj );
    virtual bool RemItemPrivate( void* pObj );
    virtual void SwapItemPrivate( int i, int j );
    virtual void ClearPrivate( bool bInConnection = false );
    virtual void InvalidatePrivate( void* pObj, bool bDel = false );
    virtual void push_back( T* const & x );

public:
    //! @name Assignment/Copy constructor
    //@{
    ADN_Type_Vector_ABC< T >( const ADN_Type_Vector_ABC< T >& o );
    ADN_Type_Vector_ABC< T >& operator=( const ADN_Type_Vector_ABC< T >& o );
    //@}

protected:
    bool bAutoRef_;
};

#include "ADN_Type_Vector_ABC.inl"

#endif // __ADN_Type_Vector_ABC_h_