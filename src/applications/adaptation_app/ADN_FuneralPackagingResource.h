// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_FuneralPackagingResource_h_
#define __ADN_FuneralPackagingResource_h_

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Equipement_Data.h"

class ADN_FuneralPackagingResource;

typedef ADN_Type_Vector_ABC< ADN_FuneralPackagingResource >  T_FuneralPackagingResource_Vector;
typedef T_FuneralPackagingResource_Vector::iterator         IT_FuneralPackagingResource_Vector;

namespace xml { class xistream; }
class xml::xostream;

// =============================================================================
/** @class  ADN_FuneralPackagingResource
    @brief  ADN_FuneralPackagingResource
*/
// Created: SBO 2006-08-04
// =============================================================================
class ADN_FuneralPackagingResource
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_FuneralPackagingResource();
    virtual ~ADN_FuneralPackagingResource();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    std::string GetItemName();

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    void WriteArchive( xml::xostream& output, const std::string& attribute, const std::string& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_FuneralPackagingResource( const ADN_FuneralPackagingResource& );            //!< Copy constructor
    ADN_FuneralPackagingResource& operator=( const ADN_FuneralPackagingResource& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo> resource_;
    ADN_Type_Time processDuration_;
    ADN_Type_Bool terminal_;
    //@}
};

#endif // __ADN_FuneralPackagingResource_h_
