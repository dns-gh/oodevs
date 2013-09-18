// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Communications_Data_h_
#define __ADN_Communications_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"

// =============================================================================
/** @class  ADN_Communications_Data
    @brief  ADN_Communications_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Communications_Data : public ADN_Data_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Communications_Data();
    virtual ~ADN_Communications_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ADN_Type_Double rSpeedModifier_;
    ADN_Type_Double rReloadModifier_;
};

#endif // __ADN_Communications_Data_h_
