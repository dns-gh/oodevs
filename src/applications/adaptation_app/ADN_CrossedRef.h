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
             ADN_CrossedRef( const ADN_Type_Vector_ABC< T >& vector, T* element = 0, bool linked = false, const std::string& xmlAttributeName = "name" );
    virtual ~ADN_CrossedRef();
    //@}

    //! @name Operations
    //@{
    void SetCrossedElement( T* element );
    T* GetCrossedElement() const;
    const ADN_Type_Vector_ABC< T >& GetVector() const;
    virtual void ReadArchive( xml::xistream& xis );
    virtual void WriteArchive( xml::xostream& xos );
    using ADN_Ref_ABC::CheckValidity;
    virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );
    //@}

private:
    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC< T > ptr_;
    std::string xmlAttributeName_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef constructor
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
ADN_CrossedRef< T >::ADN_CrossedRef( const ADN_Type_Vector_ABC< T >& vector, T* element /* = 0 */, bool linked /* = false */, const std::string& xmlAttributeName /*= "name"*/ )
    : ADN_RefWithName()
    , ptr_( vector, 0 )
    , xmlAttributeName_( xmlAttributeName )
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
// Name: ADN_Type_Vector_ABC< T >& ADN_CrossedRef::GetVector
// Created: ABR 2013-02-11
// -----------------------------------------------------------------------------
template< typename T >
const ADN_Type_Vector_ABC< T >& ADN_CrossedRef< T >::GetVector() const
{
    return ptr_.GetVector();
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
void ADN_CrossedRef< T >::SetCrossedElement( T* element )
{
    ptr_ = element;
    if( element )
        strName_.Connect( &ptr_.GetData()->strName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::ReadArchive
// Created: JSR 2013-04-09
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( xmlAttributeName_, ptr_ );
    if( ptr_.GetData() )
        strName_.Connect( &ptr_.GetData()->strName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_CrossedRef::WriteArchive
// Created: JSR 2013-04-09
// -----------------------------------------------------------------------------
template< typename T >
void ADN_CrossedRef< T >::WriteArchive( xml::xostream& xos )
{
    xos << xml::attribute( xmlAttributeName_, ptr_ );
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


#endif // __ADN_CrossedRef_h_
