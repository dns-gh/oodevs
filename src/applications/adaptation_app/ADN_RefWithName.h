// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_RefWithName_h_
#define __ADN_RefWithName_h_

#include "ADN_Ref_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_LocalizedString.h"

// =============================================================================
/** @class  ADN_RefWithName
    @brief  ADN_RefWithName
*/
// Created: ABR 2012-11-05
// =============================================================================
template< typename T >
class ADN_RefWithText : public ADN_Ref_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_RefWithText() : ADN_Ref_ABC(), strName_() {}
    explicit ADN_RefWithText( const std::string& name ) : ADN_Ref_ABC(), strName_( name ) {}
    explicit ADN_RefWithText( const QString& name ) : ADN_Ref_ABC(), strName_( name.toStdString() ) {}
    explicit ADN_RefWithText( ADN_RefWithText& other ) : ADN_Ref_ABC(), strName_( other.strName_.GetData() ) {}
    explicit ADN_RefWithText( ADN_RefWithText* otherPtr) : ADN_Ref_ABC(), strName_() { assert( otherPtr ); strName_ = otherPtr->strName_.GetData(); }
    virtual ~ADN_RefWithText() {}
    //@}

    //! @name Operations
    //@{
    using ADN_Ref_ABC::CheckValidity;
    virtual void CheckValidity()
    {
        ADN_Ref_ABC::CheckValidity();
        CheckTypeValidity( strName_ );
    }

public:
    //! @name Member data
    //@{
    T strName_;
    //@}
};

// -----------------------------------------------------------------------------
// Stream operators
// -----------------------------------------------------------------------------
template< typename T >
std::ostream& operator<<( std::ostream& os, const ADN_RefWithText< T >& type )
{
    return os << type.strName_;
}

template< typename T >
xml::xostream& operator<<( xml::xostream& xos, const ADN_RefWithText< T >& type )
{
    return xos << type.strName_;
}
template< typename T >
xml::xistream& operator>>( xml::xistream& xis, ADN_RefWithText< T >& type )
{
    xis >> type.strName_;
    return xis;
}

typedef ADN_RefWithText< ADN_Type_String > ADN_RefWithName;
typedef ADN_RefWithText< ADN_Type_LocalizedString > ADN_RefWithLocalizedName;

#endif // __ADN_RefWithName_h_
