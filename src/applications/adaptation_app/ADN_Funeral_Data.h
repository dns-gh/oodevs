// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Funeral_Data_h_
#define __ADN_Funeral_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_FuneralPackagingResource.h"

// =============================================================================
/** @class  ADN_Funeral_Data
    @brief  ADN_Funeral_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Funeral_Data : public ADN_Data_ABC
{
// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Funeral_Data();
    virtual ~ADN_Funeral_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadFuneralPackagingResource( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_FuneralPackagingResource_Vector funeralPackagingResources_;
    ADN_Type_Int fakeTransporterSpeed_;
};

#endif // __ADN_Funeral_Data_h_
