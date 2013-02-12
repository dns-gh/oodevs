// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TypePtr_InVector_ABC_h_
#define __ADN_TypePtr_InVector_ABC_h_

#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Type_Vector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
template <class T>
class ADN_TypePtr_InVector_ABC : public ADN_Connector_Vector_ABC
{

public:
    typedef typename T*                      T_TypePtr;
    typedef typename ADN_Type_Vector_ABC<T>  T_TypeVector;

public:
             ADN_TypePtr_InVector_ABC();
    explicit ADN_TypePtr_InVector_ABC( const T_TypePtr& value );
    explicit ADN_TypePtr_InVector_ABC( const T_TypeVector& v, const T_TypePtr& value );
    virtual ~ADN_TypePtr_InVector_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize( ADN_Connector_Vector_ABC& dest ) const;
    virtual void Initialize( ADN_Connector_ABC& dest ) const;

    void SetData(const T_TypePtr& value );
    const T_TypePtr GetData() const;

    void          SetVector(const T_TypeVector& v);
    const T_TypeVector& GetVector() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
//    ADN_TypePtr_InVector_ABC& operator =(const ADN_TypePtr_InVector_ABC& o);
    ADN_TypePtr_InVector_ABC& operator =(const T_TypePtr& val);
    bool          operator ==(const ADN_TypePtr_InVector_ABC& val) const;
    bool          operator ==(const T_TypePtr& val) const;
    //@}

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual void  SetDataPrivate(void *data);
    virtual void  InvalidatePrivate( void *ptr ,bool bDel=false);
    virtual bool  AddItemPrivate(void *item );
    virtual bool  RemItemPrivate(void *item );

private:
    T_TypePtr       pData_;
    T_TypeVector*   pVector_;
};

#include "ADN_TypePtr_InVector_ABC.inl"

#endif // __ADN_TypePtr_InVector_ABC_h_