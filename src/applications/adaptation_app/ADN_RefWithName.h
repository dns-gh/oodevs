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
#include "ADN_LocalizedTypes.h"

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
        ADN_ErrorStatus status = GetErrorStatus();
        strName_.CheckValidity();
        status = std::max< ADN_ErrorStatus >( status, strName_.GetErrorStatus() );
        SetErrorStatus( status );
    }

    virtual void OnLanguageChanged( const std::string& language )
    {
        strName_.OnLanguageChanged( language );
    }
    //@}

public:
    //! @name Member data
    //@{
    T strName_;
    //@}
};


typedef ADN_RefWithText< ADN_Type_String > ADN_RefWithName;
typedef ADN_RefWithText< ADN_Type_LocalizedString > ADN_RefWithLocalizedName;


#endif // __ADN_RefWithName_h_
