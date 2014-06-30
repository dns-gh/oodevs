// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_CrossedRef_h_
#define __ADN_CrossedRef_h_

#include "ADN_RefWithName.h"

// =============================================================================
/** @class  ADN_CrossedRef
    @brief  ADN_CrossedRef
*/
// Created: ABR 2013-02-11
// =============================================================================
template< typename T >
class ADN_CrossedRef : public ADN_RefWithName
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_CrossedRef( const ADN_Type_Vector_ABC< T >& vector, T* element = 0, bool linked = false );
    virtual ~ADN_CrossedRef();
    //@}

    //! @name Operations
    //@{
    void SetCrossedElement( T* element, bool connect = true );
    T* GetCrossedElement() const;
    const ADN_Type_Vector_ABC< T >& GetVector() const;
    void SetVector( const ADN_Type_Vector_ABC< T >& vector );
    void DisconnectName();
    void Swap( ADN_CrossedRef< T >& other );
    using ADN_Ref_ABC::CheckValidity;
    virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    //@}

private:
    //! @name Friends
    //@{
    template< typename U > friend std::ostream& operator<<( std::ostream& os, const ADN_CrossedRef< U >& type );
    template< typename U > friend xml::xostream& operator<<( xml::xostream& xos, const ADN_CrossedRef< U >& type );
    template< typename U > friend xml::xistream& operator>>( xml::xistream& xos, ADN_CrossedRef< U >& type );
    //@}

    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC< T > ptr_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef constructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
ADN_CrossedRef< T >::ADN_CrossedRef( const ADN_Type_Vector_ABC< T >& vector, T* element /* = 0 */, bool linked /* = false */ )
    : ADN_RefWithName()
    , ptr_( vector, 0 )
{
    SetCrossedElement( element );
    if( linked )
        this->BindExistenceTo( &ptr_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef destructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
ADN_CrossedRef< T >::~ADN_CrossedRef()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::GetVector
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
const ADN_Type_Vector_ABC< T >& ADN_CrossedRef< T >::GetVector() const
{
    return ptr_.GetVector();
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::SetVector
// Created: JSR 2013-04-10
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::SetVector( const ADN_Type_Vector_ABC< T >& vector )
{
    ptr_.SetVector( vector );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::GetElement
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_CrossedRef< T >::GetCrossedElement() const
{
    return ptr_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::SetElement
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::SetCrossedElement( T* element, bool connect /*= true*/ )
{
    ptr_ = element;
    if( connect && element )
        strName_.Connect( &ptr_.GetData()->strName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::CheckValidity
// Created: JSR 2013-04-03
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab /*= -1*/, const std::string& optional /*= ""*/ )
{
    ptr_.CheckValidity( checker, name, tab, subTab, optional );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::DisconnectName
// Created: ABR 2013-11-26
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::DisconnectName()
{
    if( ptr_.GetData() )
        strName_.Disconnect( &ptr_.GetData()->strName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::Swap
// Created: ABR 2013-11-26
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::Swap( ADN_CrossedRef< T >& other )
{
    ADN_TypePtr_InVector_ABC< T > ptrCopy( GetVector(), GetCrossedElement() );
    DisconnectName();
    SetVector( other.GetVector() );
    SetCrossedElement( other.GetCrossedElement() );
    other.DisconnectName();
    other.SetVector( ptrCopy.GetVector() );
    other.SetCrossedElement( ptrCopy.GetData() );
}

// -----------------------------------------------------------------------------
// Stream operators
// -----------------------------------------------------------------------------
template< typename T >
std::ostream& operator<<( std::ostream& os, const ADN_CrossedRef< T >& type )
{
    return os << type.ptr_;
}

template< typename T >
xml::xostream& operator<<( xml::xostream& xos, const ADN_CrossedRef< T >& type )
{
    return xos << type.ptr_;
}

template< typename T >
xml::xistream& operator>>( xml::xistream& xis, ADN_CrossedRef< T >& type )
{
    xis >> type.ptr_;
    if( type.GetCrossedElement() )
        type.strName_.Connect( &type.GetCrossedElement()->strName_ );
    return xis;
}

#endif // __ADN_CrossedRef_h_
