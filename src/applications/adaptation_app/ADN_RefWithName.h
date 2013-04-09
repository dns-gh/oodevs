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

// =============================================================================
/** @class  ADN_RefWithName
    @brief  ADN_RefWithName
*/
// Created: ABR 2012-11-05
// =============================================================================
class ADN_RefWithName : public ADN_Ref_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_RefWithName() : ADN_Ref_ABC(), strName_() {}
    explicit ADN_RefWithName( const std::string& name ) : ADN_Ref_ABC(), strName_( name ) {}
    explicit ADN_RefWithName( const QString& name ) : ADN_Ref_ABC(), strName_( name.toStdString() ) {}
    explicit ADN_RefWithName( ADN_RefWithName& other ) : ADN_Ref_ABC(), strName_( other.strName_.GetData() ) {}
    explicit ADN_RefWithName( ADN_RefWithName* otherPtr) : ADN_Ref_ABC(), strName_() { assert( otherPtr ); strName_ = otherPtr->strName_.GetData(); }
    virtual ~ADN_RefWithName() {}
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
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_String strName_;
    //@}
};

#endif // __ADN_RefWithName_h_
