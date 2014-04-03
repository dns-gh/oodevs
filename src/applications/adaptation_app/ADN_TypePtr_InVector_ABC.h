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
#include "ADN_ConsistencyChecker.h"
#include "ADN_Tools.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Workspace.h"

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
template <class T>
class ADN_TypePtr_InVector_ABC : public ADN_Connector_Vector_ABC
{

public:
    typedef typename T* T_TypePtr;
    typedef typename ADN_Type_Vector_ABC< T > T_TypeVector;

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
    void Initialize( T_TypeVector* vector = 0 );
    void SetRefName( const std::string& refName );
    const std::string& GetRefName() const;

    void SetData( const T_TypePtr& value );
    const T_TypePtr GetData() const;

    void SetVector( const T_TypeVector& v );
    const T_TypeVector& GetVector() const;

    virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators*/
    //-------------------------------------------------------------------------
    //@{
    ADN_TypePtr_InVector_ABC& operator = ( const T_TypePtr& val );
    bool operator == ( const ADN_TypePtr_InVector_ABC& val ) const;
    bool operator == ( const T_TypePtr& val ) const;
    //@}

protected:
    virtual void ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );
    virtual void DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget );

    virtual void SetDataPrivate( void* data );
    virtual void InvalidatePrivate( void* ptr, bool bDel = false );
    virtual bool AddItemPrivate( void* item );
    virtual bool RemItemPrivate( void* item );

    virtual void ClearPrivate( bool bInConnection = false );

private:
    T_TypePtr pData_;
    T_TypeVector* pVector_;
    std::string refName_;
};

#include "ADN_TypePtr_InVector_ABC.inl"

// -----------------------------------------------------------------------------
// Stream operators
// -----------------------------------------------------------------------------
template< typename T >
std::ostream& operator<<( std::ostream& os, const ADN_TypePtr_InVector_ABC< T >& type )
{
    if( type.GetData() )
        return os << *static_cast< T* >( type.GetData() );
    return os;
}

template< typename T >
xml::xostream& operator<<( xml::xostream& xos, const ADN_TypePtr_InVector_ABC< T >& type )
{
    if( type.GetData() )
        return xos << *static_cast< T* >( type.GetData() );
    return xos;
}
template< typename T >
xml::xistream& operator>>( xml::xistream& xis, ADN_TypePtr_InVector_ABC< T >& type )
{
    std::string value;
    xis >> value;
    type.SetRefName( value );
    type.Initialize();
    return xis;
}

#endif // __ADN_TypePtr_InVector_ABC_h_
